cmake_minimum_required(VERSION 2.8.3)
project(plastun_cascad_testing)

SET (CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
sensor_msgs
cv_bridge
roscpp
rospy
std_msgs
image_transport
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${cv_bridge_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)


catkin_package()
include_directories(src)
add_executable(cascad_testing src/cascad_testing.cpp src/main.cpp)
target_link_libraries(cascad_testing ${catkin_LIBRARIES} ${cv_bridge_LIBRARIES})

#add_dependencies(cascad_testing ${plastun_cascad_testing_EXPORTED_TARGETS})


