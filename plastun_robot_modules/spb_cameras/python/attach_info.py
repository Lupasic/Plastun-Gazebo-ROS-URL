#!/usr/bin/env python
#
#
import rospy
from sensor_msgs.msg import Image, CameraInfo
import camera_info_manager
import string

class attach_info:
    def __init__(self, _prefix, camera_name, info_prefix, cinfo_url):
        self.name = camera_name
        self.uri = cinfo_url
        self.prefix = _prefix

        self.cname = camera_info_manager.genCameraName(self.prefix)
        rospy.loginfo("cname = "+self.cname)
        self.cinfo = camera_info_manager.CameraInfoManager(cname = _prefix,
                                                   url = self.uri,
                                                   namespace=info_prefix)
        self.caminfo_pub = rospy.Publisher(info_prefix + "/camera_info", CameraInfo, self)
        self.cinfo.loadCameraInfo()
        rospy.Subscriber(self.prefix+self.name, Image, self.image_callback)

    def peer_subscribe(self, topic_name, topic_publish, peer_publish):
         rospy.loginfo("subscribe to " + topic_name)

    def image_callback(self, data):
        #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.header)
        cimsg = self.cinfo.getCameraInfo()
        cimsg.header.stamp = data.header.stamp
        cimsg.header.frame_id = data.header.frame_id
        cimsg.width = data.width
        cimsg.height = data.height
        self.caminfo_pub.publish(cimsg)


def main():

    nodename = "attach_info"
    node = rospy.init_node(nodename)

    args_default = {
        'prefix': 'my/',       # default IP address
        'image': 'image',      # default login name
        'info prefix': 'info',
        'uri':   ''
      }

    args = updateArgs(args_default)
    print 'settings'
    print args
    rospy.loginfo("subscribe to " + args['prefix']+args['image'])

    attach = attach_info(args['prefix'], args['image'], args['info prefix'], args['uri'])
    rospy.spin()

def updateArgs(arg_defaults):
    '''Look up parameters starting in the driver's private parameter space, but
    also searching outer namespaces.  Defining them in a higher namespace allows
    the axis_ptz.py script to share parameters with the driver.'''
    args = {}
    for name, val in arg_defaults.iteritems():
        full_name = rospy.search_param(name) #search without postfix
        if full_name is None:   #search with postfix
            full_name = rospy.search_param(name)
        if full_name is None:  #use default
            args[name] = val
        else:
            args[name] = rospy.get_param(full_name, val)

    return(args)

if __name__ == "__main__":
    main()