cmake_minimum_required(VERSION 2.8.3)
project(plastun_system_control)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  message_generation
  roscpp
  rospy
  std_msgs
)

#add_executable(rotate_turret src/rotate_turret.cpp src/rotate_turret.h src/main.cpp)
#target_link_libraries(rotate_turret ${catkin_LIBRARIES})


#add_action_files(
#  DIRECTORY action
#  FILES coords.action
#)

#generate_messages(
#  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
#)