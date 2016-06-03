cmake_minimum_required(VERSION 2.8.3)
project(plastun_activate_laser)

SET (CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  message_generation
  roscpp
  rospy
  std_msgs
)


add_action_files(
  DIRECTORY action
  FILES Fire.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
)


include_directories(src)
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


add_executable(activate_laser src/activate_laser.cpp src/main.cpp src/dioAurora/ecram.cc src/dioAurora/dioport.cpp)
target_link_libraries(activate_laser ${catkin_LIBRARIES})

add_dependencies(
  activate_laser
  ${plastun_activate_laser_EXPORTED_TARGETS}
)
