#include "arm_robot_system.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"



hardware_interface::CallbackReturn ArmRobotSystemHardware::on_init(
    const hardware_interface::HardwareInfo & params)
{
     if (
    hardware_interface::SystemInterface::on_init(params) !=
    hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }
  //sprawdzamy czy kazdy joint ma interface z position i velocity
  for(const auto & joint : info_.joints)
  {
    if(joint.command_interfaces.size() != 2)
    {
         RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' has %zu command interfaces found. 2 expected.",
        joint.name.c_str(), joint.command_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

     if (joint.command_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' have %s command interfaces found. '%s' expected.",
        joint.name.c_str(), joint.command_interfaces[0].name.c_str(),
        hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }

       if (joint.command_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' have %s command interfaces found. '%s' expected.",
        joint.name.c_str(), joint.command_interfaces[1].name.c_str(),
        hardware_interface::HW_IF_VELOCITY);
      return hardware_interface::CallbackReturn::ERROR;
    }



     if (joint.state_interfaces.size() != 2)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' has %zu state interface. 2 expected.", joint.name.c_str(),
        joint.state_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' have '%s' as first state interface. '%s' expected.",
        joint.name.c_str(), joint.state_interfaces[0].name.c_str(),
        hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger("ArmRobotSystemHardware"), "Joint '%s' have '%s' as second state interface. '%s' expected.",
        joint.name.c_str(), joint.state_interfaces[1].name.c_str(),
        hardware_interface::HW_IF_VELOCITY);
      return hardware_interface::CallbackReturn::ERROR;
    }
  }
  //inicjalizacja wektorow na dane
    auto size = info_.joints.size();
    velocities_.resize(size);
    positions_.resize(size);
    hw_velocities_.resize(size);
    hw_positions_.resize(size);

  //wszystko gitarka
  return hardware_interface::CallbackReturn::SUCCESS;
}
    
  hardware_interface::CallbackReturn ArmRobotSystemHardware::on_configure(
    const rclcpp_lifecycle::State &)
  {
    RCLCPP_INFO(rclcpp::get_logger("ArmRobotSystemHardware"), "Configuring Arm ...wait....");

    std::string port= "/dev/ttyACM0";

    if (!uart_.connect(port, 115200)) {
        RCLCPP_FATAL(rclcpp::get_logger("ArmRobotSystemHardware"), "Failed to connect to UART port: %s", port.c_str());
        return hardware_interface::CallbackReturn::ERROR;
    }

      for(uint i = 0; i < hw_positions_.size(); i++) {
        hw_positions_[i] = 0.0;
        hw_velocities_[i] = 0.0;
        positions_[i] = 0.0;
        velocities_[i] = 0.0;
    }
        
    RCLCPP_INFO(rclcpp::get_logger("ArmRobotSystemHardware"), "Successfully configured!");
    return hardware_interface::CallbackReturn::SUCCESS;
    }


    //test
     hardware_interface::CallbackReturn ArmRobotSystemHardware::on_activate(
    const rclcpp_lifecycle::State & /*previous_state*/)
    {
    RCLCPP_INFO(
        rclcpp::get_logger("ArmRobotSystemHardware"),
        "Aktywacja interfejsu sprzetowego");

    return hardware_interface::CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn ArmRobotSystemHardware::on_deactivate(
    const rclcpp_lifecycle::State & /*previous_state*/)
    {
    RCLCPP_INFO(
        rclcpp::get_logger("ArmRobotSystemHardware"),
        "Dezaktywacja interfejsu sprzetowego");

    return hardware_interface::CallbackReturn::SUCCESS;
    }

std::vector<hardware_interface::StateInterface> ArmRobotSystemHardware::export_state_interfaces()
{
    std::vector<hardware_interface::StateInterface> state_interfaces;
    for(auto i = 0u; i < info_.joints.size(); i++)
    {
        state_interfaces.emplace_back(hardware_interface::StateInterface(
            info_.joints[i].name, hardware_interface::HW_IF_POSITION,&hw_positions_[i]
        ));
         state_interfaces.emplace_back(hardware_interface::StateInterface(
            info_.joints[i].name, hardware_interface::HW_IF_VELOCITY,&hw_velocities_[i]
        ));
    }
    return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ArmRobotSystemHardware::export_command_interfaces()
{   
    std::vector<hardware_interface::CommandInterface> command_interfaces;
       for(auto i = 0u; i < info_.joints.size(); i++)
    {
        command_interfaces.emplace_back(hardware_interface::CommandInterface(
            info_.joints[i].name, hardware_interface::HW_IF_POSITION,&positions_[i]
        ));
         command_interfaces.emplace_back(hardware_interface::CommandInterface(
            info_.joints[i].name, hardware_interface::HW_IF_VELOCITY,&velocities_[i]
        ));
    }
    
    return command_interfaces;
}

hardware_interface::return_type ArmRobotSystemHardware::read(const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
{
   if (uart_.receivePackage(hw_positions_, hw_velocities_))
    {
        RCLCPP_INFO(
            rclcpp::get_logger("ArmRobotSystemHardware"),
            "UART RX id 1: pos=%.3f vel=%.3f id2: pos=%.3f vel=%.3f",
            hw_positions_[0],
            hw_velocities_[0],
          hw_positions_[1],
            hw_velocities_[1]);
    }

    return hardware_interface::return_type::OK;
}


hardware_interface::return_type ArmRobotSystemHardware::write(const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
{
     const bool sent =
        uart_.parse_send_packagage(velocities_, positions_);

    return sent
        ? hardware_interface::return_type::OK
        : hardware_interface::return_type::ERROR;
}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(ArmRobotSystemHardware, hardware_interface::SystemInterface)