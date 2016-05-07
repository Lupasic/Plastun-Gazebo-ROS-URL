cmake_minimum_required(VERSION 2.8.3)
project(plastun_image_detect)

SET (CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
sensor_msgs
actionlib
actionlib_msgs
message_generation
cv_bridge
roscpp
rospy
std_msgs
image_transport
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_action_files(
  DIRECTORY action
  FILES access_detect.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs  # Or other packages containing msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${cv_bridge_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIRS}
)

#catkin_package()
include_directories(src)
add_executable(image_detect src/image_detect.cpp src/main.cpp)
target_link_libraries(image_detect ${catkin_LIBRARIES} ${cv_bridge_LIBRARIES})

add_dependencies(image_detect ${plastun_image_detect_EXPORTED_TARGETS})


