#pragma once

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/clock.hpp"
#include "rclcpp/duration.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "uart_comm.hpp"

class ArmRobotSystemHardware : public hardware_interface::SystemInterface , public uartcomm
{
    public:
        hardware_interface::CallbackReturn on_init(
        const hardware_interface::HardwareInfo & params) override;

        hardware_interface::CallbackReturn on_configure(
        const rclcpp_lifecycle::State & previous_state) override;

        hardware_interface::CallbackReturn on_activate(
        const rclcpp_lifecycle::State & previous_state) override;

        hardware_interface::CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State & previous_state) override;

        hardware_interface::return_type read(
        const rclcpp::Time & time, const rclcpp::Duration & period) override;

        hardware_interface::return_type write(
        const rclcpp::Time & time, const rclcpp::Duration & period) override;

        std::vector<hardware_interface::CommandInterface> ArmRobotSystemHardware::export_command_interfaces() override;

        std::vector<hardware_interface::StateInterface> ArmRobotSystemHardware::export_state_interfaces() override;


        hardware_interface::return_type read(
        const rclcpp::Time & time, const rclcpp::Duration & period) override;

        hardware_interface::return_type write(
        const rclcpp::Time & time, const rclcpp::Duration & period) override;

    private:
        std::vector<double> velocities_;
        std::vector<double> positions_;

        std::vector<double> hw_velocities_;
        std::vector<double> hw_positions_;
};