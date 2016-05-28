cmake_minimum_required(VERSION 2.8.3)
project(bins)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  common
  nav_msgs
  roscpp
  std_msgs
  tf
 )

catkin_package()

find_package( common COMPONENTS
        common
        )

include_directories(  ${catkin_INCLUDE_DIRS}
  ${common_INCLUDE_DIRS}
)

add_executable(bins src/ahrs.cc src/AHRSManager.cpp )

target_link_libraries(bins   ${catkin_LIBRARIES}  ${common_LIBRARIES} )

# Install targets
install(TARGETS bins
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
