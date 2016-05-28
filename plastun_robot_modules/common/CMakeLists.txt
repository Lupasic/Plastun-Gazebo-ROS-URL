cmake_minimum_required(VERSION 2.8.3)
project(common)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

## System dependencies are found with CMake's conventions
#find_package(zmq)
#find_package(Protobuf REQUIRED)
#include_directories(${PROTOBUF_INCLUDE_DIRS})



catkin_package(
  INCLUDE_DIRS include # MessageQueue ${zmq_INCLUDE_DIRS}
    LIBRARIES common # MessageQueue ${zmq_LIBRARIES}
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)

## Declare a cpp library
 add_library(common
   src/Archive.cpp
   src/matrixHelper.cpp
   src/Socket.cpp
   src/TIORoutines.cpp
   src/NormalRand.cpp
   src/TFile.cpp
   src/TriGeom.cpp
   src/Geom.cpp
   src/Observer.cpp
   src/TimeProcs.cpp
   src/TThread.cpp
   src/inifile.cpp
   src/SerializeHelper.cpp
   src/timer.cpp
   src/crc/TCRC16_CCITT.cpp
   src/crc/tcrc16.cpp
   src/crc/TCRC16LHA.cpp
   src/crc/TCRC16_Z_modem.cpp
   src/crc/tcrc32.cpp
 )


#include_directories(${CMAKE_
#add_library(DistrMessages
#   ${PROTO_SRCS} ${PROTO_HDRS}
#   )

 
#add_library(MessageQueue
#   MessageQueue/ConnectionFactory.cpp
#   MessageQueue/ZReceiveQueue.cpp
#   MessageQueue/ZSendQueue.cpp
#   )

target_link_libraries(common rt)
#target_link_libraries(DistrMessages protobuf)
#target_link_libraries(MessageQueue DistrMessages)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

install(TARGETS common  #MessageQueue
        RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})



## Mark cpp header files for installation
 install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.h"
   PATTERN ".svn" EXCLUDE
 )



#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_common.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
