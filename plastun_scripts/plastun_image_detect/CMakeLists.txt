cmake_minimum_required(VERSION 2.8.3)
project(plastun_image_detect)



SET (CMAKE_BUILD_TYPE Debug)
find_package(catkin REQUIRED COMPONENTS
sensor_msgs
cv_bridge
roscpp
std_msgs
image_transport
)

#if(NOT OpenCV_FOUND)
 #       message(SEND_ERROR "Failed to find OpenCV")
#        return()
#else()
 #       include_directories(${OpenCV_INCLUDE_DIRS})
#endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${cv_bridge_INCLUDE_DIRS}
)

add_executable(image_converter src/image_converter.cpp src/image_converter.h src/main.cpp)
target_link_libraries(image_converter ${catkin_LIBRARIES} ${cv_bridge_LIBRARIES})
add_dependencies(image_converter image_converter_cpp)
