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
        rclcpp::get_logger, "Joint '%s' has %zu command interfaces found. 2 expected.",
        joint.name.c_str(), joint.command_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

     if (joint.command_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger, "Joint '%s' have %s command interfaces found. '%s' expected.",
        joint.name.c_str(), joint.command_interfaces[0].name.c_str(),
        hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }

       if (joint.command_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger(), "Joint '%s' have %s command interfaces found. '%s' expected.",
        joint.name.c_str(), joint.command_interfaces[1].name.c_str(),
        hardware_interface::HW_IF_VELOCITY);
      return hardware_interface::CallbackReturn::ERROR;
    }



     if (joint.state_interfaces.size() != 2)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger(), "Joint '%s' has %zu state interface. 2 expected.", joint.name.c_str(),
        joint.state_interfaces.size());
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces[0].name != hardware_interface::HW_IF_POSITION)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger(), "Joint '%s' have '%s' as first state interface. '%s' expected.",
        joint.name.c_str(), joint.state_interfaces[0].name.c_str(),
        hardware_interface::HW_IF_POSITION);
      return hardware_interface::CallbackReturn::ERROR;
    }

    if (joint.state_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
    {
      RCLCPP_FATAL(
        rclcpp::get_logger(), "Joint '%s' have '%s' as second state interface. '%s' expected.",
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
         RCLCPP_INFO(rclcpp::get_logger(), "Configurin Arm ...wait....");
    
        for( const auto & [name,desr] : joint_state_interfaces_)
        {
            set_state(name, 0.0);
        }  
        
        for (const auto & [name, descr] : joint_command_interfaces_)
        {
            set_command(name, 0.0);
        }
        
        RCLCPP_INFO(rclcpp::get_logger(), "Successfully configured!");

       
  
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

hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period)
{

}

hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period)
{

} 