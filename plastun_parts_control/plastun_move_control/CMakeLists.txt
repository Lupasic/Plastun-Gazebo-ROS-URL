cmake_minimum_required(VERSION 2.8.3)
project(plastun_move_control)

SET (CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  message_generation
  roscpp
  rospy
  std_msgs
)

#add_action_files(
#  DIRECTORY ../plastun_rotate_turret/action
#  FILES angle.action
#)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
)

include_directories(src)
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


add_executable(move_control src/move_control.cpp src/main.cpp)
target_link_libraries(move_control ${catkin_LIBRARIES})
