cmake_minimum_required(VERSION 2.8.3)
project(odo_tf_pub)

find_package(catkin REQUIRED COMPONENTS
  nav_msgs
  roscpp
  sensor_msgs
  tf
)

catkin_package()

include_directories(  ${catkin_INCLUDE_DIRS})

add_executable(odo_tf_pub src/tf_broadcaster.cpp)

add_dependencies(odo_tf_pub odo_tf_pub_generate_messages_cpp)

target_link_libraries(odo_tf_pub   ${catkin_LIBRARIES} )