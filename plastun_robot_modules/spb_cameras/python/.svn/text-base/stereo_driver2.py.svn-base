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
        #self.axis_str = str(axis)
        self.daemon = True
        self.timeoutSeconds = 2.5


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
	print "url = ",self.url
        #self.url += "?fps=0&resolultion=%dx%d" % (self.axis.width,
         #                                                   self.axis.height)
        rospy.logdebug('opening ' + str(self.axis))

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
#                 self.findBoundary()
# 		        rospy.loginfo('start read %s' % self.url)
                self.getImage()
		self.publishMsg()
		#rospy.loginfo('finish read %s' % self.url )
            except:
                rospy.loginfo('Timed out while trying to get message.')
                #break

    def findBoundary(self):
        '''The string "--myboundary" is used to denote the start of an image in
        Axis cameras'''
        while(True):
            boundary = self.fp.readline()
#             print 'read ', boundary
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
#         	print 'line=',line
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
	#rospy.loginfo('image ready size=' + str(self.content_length))
        self.axis.prepare_image(self.img)


class Axis:
    def __init__(self, hostname, username, password, frame_id,
                                                            camera_info_url, prefix):
        self.hostname = hostname
        self.username = username
        self.password = password
        self.frame_id = frame_id
        self.camera_info_url = camera_info_url

        self.msg = CompressedImage()
        self.msg.header.frame_id = self.frame_id
        self.msg.format = "jpeg"
        self.valid = False;
        self.updated = False;

        # generate a valid camera name based on the hostname
        self.cname = camera_info_manager.genCameraName(self.hostname)
        self.cinfo = camera_info_manager.CameraInfoManager(cname = self.cname,
                                                   url = self.camera_info_url, namespace = prefix)
        self.cinfo.loadCameraInfo()         # required before getCameraInfo()
        self.st = StreamThread(self)
        self.st.start()
        self.pub = rospy.Publisher(prefix + "/image_raw/compressed", CompressedImage, self)
        self.caminfo_pub = rospy.Publisher(prefix + "/camera_info", CameraInfo, self)
        self.detect_dim()

    def __str__(self):
        """Return string representation."""
        return(self.hostname )

    def peer_subscribe(self, topic_name, topic_publish, peer_publish):
        '''Lazy-start the image-publisher.'''
        if self.st is None:
            self.st = StreamThread(self)
            self.st.start()

    def prepare_image(self, data):
        self.msg.data = data
        self.valid = True;
        self.updated = True;

    def publish_image(self, time):
	'''Publish camera image with time'''
        if self.valid:
	    self.msg.header.stamp = time
	    self.pub.publish(self.msg)
            res = self.updated
            self.updated = False
            return res
        return False

    def publish_info(self, time):
        '''Publish camera info manager message'''
        cimsg = self.cinfo.getCameraInfo()
        cimsg.header.stamp = time
        cimsg.header.frame_id = self.frame_id
        cimsg.width = self.width
        cimsg.height = self.height
        self.caminfo_pub.publish(cimsg)

    def detect_dim(self):
        '''Auto detect image dimensions'''
        while not self.valid:
            print 'wait image'
            rospy.sleep(1)
        img_array = np.asarray(bytearray(self.msg.data), dtype=np.uint8)
        decimg=cv2.imdecode(img_array,1)
        self.height, self.width, self.depth = decimg.shape
        print 'detected dim', self.width, self.height


def main():

    nodename = "spb_cam_driver"
    node = rospy.init_node(nodename)

    args_default = {
        'hostname': '192.168.0.17/video2.mjpg',       # default IP address
        'username': 'Admin',               # default login name
        'password': '9999',
        'frame_id': '/stereo_camera',
        'camera_info_url': '',
        'prefix': 'right'}

    left_args = updateArgs(args_default, '_left')
    print 'left settings'
    print left_args
   

    right_args = updateArgs(args_default, '_right')

    print 'right settings'
    print right_args

    if (right_args['hostname'] == left_args['hostname']) :
        rospy.logerr('same prefix for both cameras ' + right_args['hostname'])
        return
        
    LeftCam=Axis(**left_args)
    RightCam=Axis(**right_args)

    r = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
	#print "timeout ",LeftCam.updated , RightCam.updated
        if LeftCam.updated and RightCam.updated:
            time = rospy.Time.now()
            #print "publish images ", time.secs, time.nsecs
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
