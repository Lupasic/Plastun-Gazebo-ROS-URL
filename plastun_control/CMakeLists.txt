cmake_minimum_required(VERSION 2.8.3)
project(plastun_control)

find_package(catkin)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES plastun_control
#  CATKIN_DEPENDS plastun_description
#  DEPENDS system_lib
)


###########
## Build ##
###########


include_directories(
  ${catkin_INCLUDE_DIRS}
)
