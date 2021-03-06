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
import numpy as np

  
class StreamThread(threading.Thread):
    def __init__(self, axis):
        threading.Thread.__init__(self)
        self.axis = axis
        self.axis_str = str(axis)
        self.daemon = True
        self.timeoutSeconds = 2.5
	self.dim_detected = 0

    def run(self):
        while(True):
            self.stream()

    def stream(self):
        while(True):
            self.formURL()
            self.authenticate()
            if self.openURL():
                self.publishFramesContinuously()
            rospy.sleep(2) # if stream stays intact we shouldn't get to this

    def formURL(self):
        self.url = 'http://%s' % self.axis.hostname
	#print "url = ",self.url
        #self.url += "?fps=0&resolultion=%dx%d" % (self.axis.width, 
         #                                                   self.axis.height)
        rospy.loginfo('opening ' + str(self.axis))

    def authenticate(self):
        '''only try to authenticate if user/pass configured.  I have not
        used this method (yet).'''
        if self.axis.password != '' and self.axis.username != '':
            # create a password manager
            password_mgr = urllib2.HTTPPasswordMgrWithDefaultRealm()

            # Add the username and password, use default realm.
            top_level_url = "http://" + self.axis.hostname
            password_mgr.add_password(None, top_level_url, self.axis.username, 
                                                            self.axis.password)
            handler = urllib2.HTTPBasicAuthHandler(password_mgr)

            # create "opener" (OpenerDirector instance)
            opener = urllib2.build_opener(handler)

            # ...and install it globally so it can be used with urlopen.
            urllib2.install_opener(opener)
    
    def openURL(self):
        '''Open connection to Axis camera using http'''
        try:
            self.fp = urllib2.urlopen(self.url, timeout=self.timeoutSeconds)
            return(True)
        except urllib2.URLError, e:
            rospy.logwarn('Error opening URL %s' % (self.url) +
                            ' Possible timeout.  Looping until camera appears')
	    print "error open"
            return(False)

    def publishFramesContinuously(self):
        '''Continuous loop to publish images'''
        while(True):
            try:
                self.findBoundary()
                self.getImage()
		if ( self.dim_detected == 0 ):
			#print "detected_dim ", self.detected_dim
		    self.detectDim()		    			
                self.publishMsg()
                self.publishCameraInfoMsg()
            except:
                rospy.loginfo('Timed out while trying to get message.')
                #print 'Timed out while trying to get message.'
                break

    def findBoundary(self):
        '''The string "--myboundary" is used to denote the start of an image in
        Axis cameras'''
        while(True):
            boundary = self.fp.readline()
            if boundary=='--myboundary\r\n':
                break

    def getImage(self):
        '''Get the image header and image itself'''
        self.getHeader()
        self.getImageData()

    def getHeader(self):
        self.header = {}
        while(True):
            line = self.fp.readline()
            if line == "\r\n":
                break
	  #  print 'line=',line
            line = line.strip()
            parts = line.split(": ", 1)
            try:
                if len(parts) == 2:
                    self.header[parts[0]] = parts[1]
            except:
                rospy.logwarn('Problem encountered with image header.  Setting '
                                                    'content_length to zero')
                self.header['Content-Length'] = 0 # set content_length to zero if 
                                            # there is a problem reading header
        self.content_length = int(self.header['Content-Length'])

    def getImageData(self):
        '''Get the binary image data itself (ie. without header)'''
        if self.content_length>0:
            self.img = self.fp.read(self.content_length)
            self.fp.readline() # Read terminating \r\n and do nothing with it

    def publishMsg(self):
        '''Publish jpeg image as a ROS message'''
        self.msg = CompressedImage()
        self.msg.header.stamp = rospy.Time.now()
        self.msg.header.frame_id = self.axis.frame_id
        self.msg.format = "jpeg"
        self.msg.data = self.img
        self.axis.pub.publish(self.msg)

    def publishCameraInfoMsg(self):
        '''Publish camera info manager message'''
        cimsg = self.axis.cinfo.getCameraInfo()
        cimsg.header.stamp = self.msg.header.stamp
        cimsg.header.frame_id = self.axis.frame_id
        cimsg.width = self.axis.width
        cimsg.height = self.axis.height
        self.axis.caminfo_pub.publish(cimsg)

    def detectDim(self):
        '''Auto detect image dimensions'''
        #print "detect dim"
        img_array = np.asarray(bytearray(self.img), dtype=np.uint8)
        decimg=cv2.imdecode(img_array,1)
        self.axis.height, self.axis.width, self.axis.depth = decimg.shape
        self.dim_detected = 1
        print 'detected dim', self.axis.width, self.axis.height

class Axis:
    def __init__(self, hostname, username, password, frame_id, 
                                                            camera_info_url):
        self.hostname = hostname
        self.username = username
        self.password = password
        self.width = 0
        self.height = 0
        self.frame_id = frame_id
        self.camera_info_url = camera_info_url

        # generate a valid camera name based on the hostname
        self.cname = camera_info_manager.genCameraName(self.hostname)
        self.cinfo = camera_info_manager.CameraInfoManager(cname = self.cname,
                                                   url = self.camera_info_url)
        self.cinfo.loadCameraInfo()         # required before getCameraInfo()
        self.st = None
        self.pub = rospy.Publisher("image_raw/compressed", CompressedImage, self)
        self.caminfo_pub = rospy.Publisher("camera_info", CameraInfo, self)

    def __str__(self):
        """Return string representation."""
        return(self.hostname + ',' + self.username + ',' + str(self.password) +
                       '(' + str(self.width) + 'x' + str(self.height) + ')')

    def peer_subscribe(self, topic_name, topic_publish, peer_publish):
        '''Lazy-start the image-publisher.'''
        if self.st is None:
            self.st = StreamThread(self)
            self.st.start()


def main():

    nodename = "spb_cam_driver"
    rospy.init_node(nodename)
    
    args_default = {
        'hostname': '192.168.0.16/video2.mjpg',       # default IP address
        'username': 'Admin',               # default login name
        'password': '9999',
        'frame_id': '/stereo_camera',
        'camera_info_url': 'file:///home/lev/Work/data/calebration/2/stereo_2_r.yaml'}
    
    args = updateArgs(args_default)
    
    print 'settings'
    print args
    Axis(**args)
    
    rospy.spin()

def updateArgs(arg_defaults):
    '''Look up parameters starting in the driver's private parameter space, but
    also searching outer namespaces.  Defining them in a higher namespace allows
    the axis_ptz.py script to share parameters with the driver.'''
    args = {}
    for name, val in arg_defaults.iteritems():
        full_name = rospy.search_param(name)
        if full_name is None:
            args[name] = val
        else:
            args[name] = rospy.get_param(full_name, val)
    # resolve frame_id with tf_prefix (unless already absolute)
    if args['frame_id'][0] != '/':        # not absolute?
        tf_prefix = rospy.search_param('tf_prefix')
        prefix_val = ''
        if tf_prefix is not None:           # prefix defined?
            prefix_val = rospy.get_param(tf_prefix)
            if prefix_val[0] != '/':          # prefix not absolute?
                prefix_val = '/' + prefix_val
        args['frame_id'] = prefix_val + '/' + args['frame_id']
    return(args)

if __name__ == "__main__":
    main()
