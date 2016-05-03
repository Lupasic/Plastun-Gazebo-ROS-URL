cmake_minimum_required(VERSION 2.8.3)
project(plastun_control_turret)


SET (CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  message_generation
  roscpp
  rospy
  std_msgs
)


generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
)

include_directories(src)
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


add_executable(control_turret src/control_turret.cpp src/main.cpp)
target_link_libraries(control_turret ${catkin_LIBRARIES})
