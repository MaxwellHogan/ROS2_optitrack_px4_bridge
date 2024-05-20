This is a ROS2 package I created to remap optitrack pose information to be used by PX4 for indoor flight experiments. 

Basic instruction to configure the FMU and provide you with backgroung are avaliable in [1] below:

[1] https://docs.px4.io/main/en/ros/external_position_estimation.html

Please note that the instructions in [1] are intended for ROS2,so only look to those for vehicle and optitrack setup.

For insturctions on installing ROS2 packages for MAVROS intergration follow the instructions in [2].

[2] https://docs.px4.io/main/en/ros/ros2.html

The ros2 package used here to access the optitrack tracking information is available from [3]:

[3]https://github.com/alvinsunyixiao/vrpn_mocap

***Known requirements:***
* https://github.com/alvinsunyixiao/vrpn_mocap
* https://github.com/PX4/px4_msgs.git
* https://github.com/PX4/px4_ros_com.git


***While I am able to get the drone to accept the positioning data, a test flight has not been performed as of 8th May 2024***

This repo is still work in progress, more functionality will be added shortly.