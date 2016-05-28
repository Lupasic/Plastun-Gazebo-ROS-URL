cmake_minimum_required(VERSION 2.8.3)
project(spb_cameras)

find_package(catkin REQUIRED
COMPONENTS
#dynamic_reconfigure
std_msgs
)

catkin_python_setup()

catkin_package()

install(PROGRAMS
python/camera_driver.py
python/stereo_driver.py
python/stereo_driver2.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(
  DIRECTORY launch
 DESTINATION ${CATKIN_GLOBAL_SHARE_DESTINATION}/${PROJECT_NAME}/
# DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN ".svn" EXCLUDE)

