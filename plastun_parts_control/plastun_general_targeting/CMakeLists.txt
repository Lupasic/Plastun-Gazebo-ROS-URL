cmake_minimum_required(VERSION 2.8.3)
project(plastun_general_targeting)

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
  FILES access_targeting.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
)


include_directories(src)
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


add_executable(general_targeting src/general_targeting.cpp src/main.cpp)
target_link_libraries(general_targeting ${catkin_LIBRARIES})

add_dependencies(
  general_targeting
  ${plastun_general_targeting_EXPORTED_TARGETS}
)