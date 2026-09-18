from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
  
    pkg_share = get_package_share_directory('robot_description')
    
  
    urdf_path = os.path.join(pkg_share, 'urdf', 'robot_description.urdf.xacro')
    yaml_path = os.path.join(pkg_share, 'config', 'robot_controllers.yaml')
    
    robot_description_content = xacro.process_file(urdf_path).toxml()

    return LaunchDescription([
        Node(
            package='controller_manager',
            executable='ros2_control_node',
            parameters=[
                {'robot_description': robot_description_content},
                yaml_path
            ],
            output='screen',
        )
    ])