cmake_minimum_required(VERSION 3.8)
project(robot_description)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(serial REQUIRED)

add_library(arm_robot_hardware_lib SHARED
  hardware/arm_robot_system.cpp
)

target_include_directories(arm_robot_hardware_lib PRIVATE hardware)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

ament_target_dependencies(arm_robot_hardware_lib 
  rclcpp 
  hardware_interface 
  pluginlib
  serial
)

install(TARGETS arm_robot_hardware_lib
  DESTINATION lib
)

install(FILES config/arm_robot_plugin.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  config
  urdf
  DESTINATION share/${PROJECT_NAME}
)

pluginlib_export_plugin_description_file(
  hardware_interface
  config/arm_robot_plugin.xml
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


ament_package()
