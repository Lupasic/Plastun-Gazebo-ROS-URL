#!/usr/bin/env python
#
# Spb camera image driver. Based on:
# https://code.ros.org/svn/wg-ros-pkg/branches/trunk_cturtle/sandbox/axis_camera
# /axis.py
#

import threading
import urllib2

import rospy 
from sensor_msgs.msg import CompressedImage, CameraInfo
import camera_info_manager
import string
import cv2
  

class Axis:
    def __init__(self, filename, frame_id, camera_info_url, prefix):
        self.filename = filename
        self.frame_id = frame_id
        self.camera_info_url = camera_info_url
        
        self.msg = CompressedImage()        
        self.msg.header.frame_id = self.frame_id
        self.msg.format = "jpeg"
	im = cv2.imread(filename)
        self.width=im.shape[1]
        self.height=im.shape[0]
	f=open(filename, 'r+')
	self.msg.data = f.read()
        self.valid = True;
        self.updated = True;

        # generate a valid camera name based on the hostname
        self.cname = camera_info_manager.genCameraName(filename)
        self.cinfo = camera_info_manager.CameraInfoManager(cname = self.cname,
                                                   url = self.camera_info_url, namespace = prefix)
        self.cinfo.loadCameraInfo()         # required before getCameraInfo()
        self.st = None
        self.pub = rospy.Publisher(prefix + "/image_raw/compressed", CompressedImage, self)
        self.caminfo_pub = rospy.Publisher(prefix + "/camera_info", CameraInfo, self)
	

    def __str__(self):
        """Return string representation."""
        return(self.hostname + ',' + self.username + ',' + str(self.password) +
                       '(' + str(self.width) + 'x' + str(self.height) + ')')

    def peer_subscribe(self, topic_name, topic_publish, peer_publish):
        '''Lazy-start the image-publisher.'''
        if self.st is None:
            self.st = StreamThread(self)
            self.st.start()
    
    def prepare_image(self, data):
        self.msg.data = data
        self.valid = true;
        self.updated = true;

    def publish_image(self, time):
	'''Publish camera image with time'''
        if self.valid:
	    
	    self.pub.publish(self.msg)
	    
            return True
        return False

    def publish_info(self, time):
        '''Publish camera info manager message'''
        cimsg = self.cinfo.getCameraInfo()
        cimsg.header.stamp = time
        cimsg.header.frame_id = self.frame_id
        cimsg.width = self.width
        cimsg.height = self.height
        self.caminfo_pub.publish(cimsg)
   


def main():

    nodename = "fake_cam_driver"
    node = rospy.init_node(nodename)
    
    args_default = {
        'filename': 'video.jpg',       # default IP address
        'frame_id': '/stereo_camera',
        'camera_info_url': '',
        'prefix': 'right'}
    
    left_args = updateArgs(args_default, '_left')
    print 'left settings'
    print left_args
    LeftCam=Axis(**left_args)

    right_args = updateArgs(args_default, '_right')
    print 'right settings'
    print right_args
    RightCam=Axis(**right_args)
    
    r = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        if LeftCam.updated and RightCam.updated:
            time = rospy.get_time()
            print "publish images"
	    LeftCam.publish_image(time)
	    RightCam.publish_image(time)
            LeftCam.publish_info(time)
            RightCam.publish_info(time)
        r.sleep()

def updateArgs(arg_defaults, postfix):
    '''Look up parameters starting in the driver's private parameter space, but
    also searching outer namespaces.  Defining them in a higher namespace allows
    the axis_ptz.py script to share parameters with the driver.'''
    args = {}
    for name, val in arg_defaults.iteritems():
	full_name = rospy.search_param(name) #search without postfix
	if full_name is None:   #search with postfix
            full_name = rospy.search_param(name+postfix)
        if full_name is None:  #use default
            args[name] = val
        else:
            args[name] = rospy.get_param(full_name, val)
    # resolve frame_id with tf_prefix (unless already absolute)
    if args['frame_id'][0] != '/':        # not absolute?
        tf_prefix = rospy.search_param('tf_prefix'+postfix)
        prefix_val = ''
        if tf_prefix is not None:           # prefix defined?
            prefix_val = rospy.get_param(tf_prefix)
            if prefix_val[0] != '/':          # prefix not absolute?
                prefix_val = '/' + prefix_val
        args['frame_id'] = prefix_val + '/' + args['frame_id']
    return(args)

if __name__ == "__main__":
    main()
