cmake_minimum_required(VERSION 2.8.3)
project(lc_ros)



## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  nav_msgs
  tf
  roscpp
  std_msgs
 )

catkin_package()

find_package( common )

include_directories(  ${catkin_INCLUDE_DIRS}
  ${common_INCLUDE_DIRS}
)

add_executable(lc_ros src/lc.cpp src/logiccontrol.cpp src/COMConnect.cpp )

add_dependencies(lc_ros  lc_ros_generate_messages_cpp)

target_link_libraries(lc_ros   ${catkin_LIBRARIES}  ${common_LIBRARIES} )

# Install targets
install(TARGETS lc_ros
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)