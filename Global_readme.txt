Общее для всего:
1) 5 Терминалов открываем
2) В 3ех из них прописываем ssh [имя аккаунта]@[адрес]. Потом вводим пароль.
3) В остальных терминалах export ROS_MASTER_URI=http://[адрес]:[порт, обычно 11311]

Gmapping:
1) В ssh, roslaunch plastun_modules_launches all_modules.launch (включение всех модулей)
2) В соседнем ssh, roslaunch plastun_navigation gmapping.launch 
3) В обычном терминале rqt (управление МРК)
4) В обычном терминале plastun_description plastun_rviz_in_gmapping.launch (для отображения построения карты)
5) В соседнем ssh, roslaunch plastun_navigation savemap.launch (сохранить построенную карту)

Для отладки move_base + amcl:
1) В ssh, roslaunch plastun_modules_launches all_modules.launch (включение всех модулей)
2) В соседнем ssh, roslaunch plastun_navigation navigation.launch 
3) В обычном терминале plastun_description plastun_rviz_in_move_base.launch (для отображения карты и МРК на ней)

Основной алгоритм:
1) В ssh, roslaunch plastun_modules_launches all_modules.launch (включение всех модулей)
2) В соседнем ssh, roslaunch plastun_launch_files_control global_control_system.launch (активация основной логики алгоритма)
3) В обычном терминале rqt (для ручного управления(если вдруг), а так же для отсмотра изображений с камеры)
4) В обычном терминале plastun_description plastun_rviz_in_control_system (для отображения карты и МРК на ней, а так же пометки цели на ней)
