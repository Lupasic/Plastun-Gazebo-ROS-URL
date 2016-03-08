^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package cob_navigation_config
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.6.3 (2015-08-31)
------------------
* cob_navigation_config: remove robots not supported anymore
* remove trailing whitespace
* migration to package format v2, indentation fixes
* Contributors: ipa-mig

0.6.2 (2015-06-17)
------------------

0.6.1 (2014-09-18)
------------------
* added cob4-2 configuration
* Contributors: ipa-cob4-2

0.6.0 (2014-09-10)
------------------

0.5.2 (2014-08-28)
------------------
* removed start_delay from scan-unifier configs and intendation-fix
* updated scan_unifier to parse scan_topics for subscription from parameter-server. added scan_unifier config-files for each robot. modified launch files for navigation and amcl to use scan_unifier
* added the new scan_unifier as well as a dummy config file and removed the old one
* added softkinetic cameras as voxel based observation sources
* Changed frame_ids to non slashed versions. TF should now work with hydro.
* Contributors: Alexander Bubeck, Florian Mirus, flg

0.5.1 (2014-03-21)
------------------
* cob_navigation: removing cob3-5b configs
* add changelog
* version bump
* remove more not needed files
* adjust CMakeLists and package.xml
* set author/maintainer email
* Update cob4-1 config
  due to @ipa-mig suggestion from https://github.com/ipa320/cob_navigation/pull/22
* Configuration for cob4-1
* change config to groovy
* add config for cob3-5b
* Catkinisation and gitignore.
* some config changes
* added navigation config for raw3-6
* modified footprint dimensions
* added raw3-3 and raw3-5 navigation config
* slower down robot
* add voxel costmap for all cob's
* integrate changes of cob_navigation into new setup
* fix navigation for cob
* allow dwa backwards movement for all cobs
* use dwa parameter for cob3-1 from janpaulus
* move planner config files from common to robot specific folder
* using voxel costmap for ros navigation
* Set clearing = true for Hokuyo sensor
* adjust footprint to fit emergency stop fields
* optimized parameters for platform movements, lowered control frequency
* add raw3-4 parameter
* optimized parameters for platform movements, lowered control frequency
* extend footpint g
* fix wiki links inf manifest.xml and stack.xml
* extend footprint
* hokuyo marking but not clearing
* extend footprint and use laser_top
* adjust footprint for desire
* extend footprint in x-direction
* removal of cob_base_velocity_smoother, moved to stack cob_driver
* namechanges from cob_vel_integrator to cob_base_velocity_smoother
* adjust manifests for documentation
* merge
* adapt max_vel_theta and footprint
* remove unavailable observation sources from raw3-2 costmap
* integrate cob_vel_integrator yaml and fix wrong inclusions
* merge branch raw3-1 into master
* default rviz config
* fix roslaunch tests
* added robot specific paramters
* removed old config file
* refactoring of cob_navigation_config; current cob_drivers required
* modifications for raw3-1
* rearrange stack
* rename cob_linear_nav to cob_navigation_linear, rearrange again
* tidy up and rearrange
* Contributors: Alexander Bubeck, Florian Weisshardt, Florian Weißhardt, IPR-SR2, abubeck, cob3-5, desire, ipa-bnm, ipa-cob3-5, ipa-fmw, ipa-frm, ipa-mig, ipa-mig-hm, robot, uh-klk

* Catkinisation and gitignore.
* some config changes
* added navigation config for raw3-6
* modified footprint dimensions
* added raw3-3 and raw3-5 navigation config
* slower down robot
* add voxel costmap for all cob's
* integrate changes of cob_navigation into new setup
* fix navigation for cob
* allow dwa backwards movement for all cobs
* use dwa parameter for cob3-1 from janpaulus
* move planner config files from common to robot specific folder
* using voxel costmap for ros navigation
* Set clearing = true for Hokuyo sensor
* adjust footprint to fit emergency stop fields
* optimized parameters for platform movements, lowered control frequency
* add raw3-4 parameter
* optimized parameters for platform movements, lowered control frequency
* extend footpint g
* fix wiki links inf manifest.xml and stack.xml
* extend footprint
* hokuyo marking but not clearing
* extend footprint and use laser_top
* adjust footprint for desire
* extend footprint in x-direction
* removal of cob_base_velocity_smoother, moved to stack cob_driver
* namechanges from cob_vel_integrator to cob_base_velocity_smoother
* adjust manifests for documentation
* merge
* adapt max_vel_theta and footprint
* remove unavailable observation sources from raw3-2 costmap
* integrate cob_vel_integrator yaml and fix wrong inclusions
* merge branch raw3-1 into master
* default rviz config
* fix roslaunch tests
* added robot specific paramters
* removed old config file
* refactoring of cob_navigation_config; current cob_drivers required
* modifications for raw3-1
* rearrange stack
* rename cob_linear_nav to cob_navigation_linear, rearrange again
* tidy up and rearrange
* Contributors: Alexander Bubeck, Florian Weißhardt, IPR-SR2, abubeck, cob3-5, desire, ipa-bnm, ipa-fmw, ipa-frm, ipa-mig, ipa-mig-hm, uh-klk
