1) Установить
gazebo-ros-control
ros-controllers 
gazebo-ros-pkgs
position-controllers 
joint-state-controller
joint-trajectory-controller

2) Запуск симуляции,  roslaunch plastun_gazebo plastun_gazebo.launch

3)Установить
move-base
amcl
gmapping
map-server

4) Если при catkin_make программа не видит сгенерированные файлы из devel, то может быть не прописан путь, решение:
export | grep 'CPATH' , если он появится то тогда может другое что то, иначе

export CPATH=/opt/ros/Твой дистрибутив/include:/Путь к твоему воркспейсу/devel/include

Добавить либо .bashrc либо как то иначе, или прийдется его прописывать при каждой перезагрузке системы

5) Запуск основной успешный сценарий
roslaunch plastun_gazebo plastun_gazebo.launch
roslaunch plastun_launch_files_control global_control_system.launch 
roslaunch plastun_description plastun_rviz_in_dynamic.launch

а) Выбрать publish point и поставить точку (первый раз установка не сработает, поэтому надо дважды ее ставить)
б) Выбрать точку, откуда будет видна цель
 
