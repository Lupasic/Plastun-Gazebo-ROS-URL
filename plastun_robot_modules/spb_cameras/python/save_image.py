#!/usr/bin/env python
#
# Spb camera image driver. Based on:
# https://code.ros.org/svn/wg-ros-pkg/branches/trunk_cturtle/sandbox/axis_camera
# /axis.py
#

import threading
import urllib2
import datetime

import string

  
class SaveImage():
    def __init__(self, hostname, username, password):
        
        self.hostname = hostname
	self.username = username
	self.password = password
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
            

    def formURL(self):
        self.url = 'http://%s' % self.hostname
	#print "url = ",self.url
        #self.url += "?fps=0&resolultion=%dx%d" % (self.axis.width, 
         #                                                   self.axis.height)

    def authenticate(self):
        '''only try to authenticate if user/pass configured.  I have not
        used this method (yet).'''
        if self.password != '' and self.username != '':
            # create a password manager
            password_mgr = urllib2.HTTPPasswordMgrWithDefaultRealm()

            # Add the username and password, use default realm.
            top_level_url = "http://" + self.hostname
            password_mgr.add_password(None, top_level_url, self.username, 
                                                            self.password)
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
	    print "error open url", self.url
            return(False)

    def publishFramesContinuously(self):
        '''Continuous loop to publish images'''
        while(True):
            try:
                self.findBoundary()
                self.getImage()
            except:
                print 'Timed out while trying to get message.'
                break

    def saveImage(self):
	name = datetime.datetime.now().strftime("images/image%Y_%m_%d_%H_%M_%S.jpg")
        print "save image ", name
        self.formURL()
        self.authenticate()
        if self.openURL():
	    self.findBoundary()
            self.getImage()
 	    if (self.content_length > 0):
		name = datetime.datetime.now().strftime("images/image%Y_%m_%d_%H_%M_%S.jpg")
		print "save image ", name
		f = open(name, 'w')
                f.write(self.img)
     	        f.close()

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
                print "Problem encountered with image header.  Setting content_length to zero"
                self.header['Content-Length'] = 0 # set content_length to zero if 
                                            # there is a problem reading header
        self.content_length = int(self.header['Content-Length'])

    def getImageData(self):
        '''Get the binary image data itself (ie. without header)'''
        if self.content_length>0:
            self.img = self.fp.read(self.content_length)
            self.fp.readline() # Read terminating \r\n and do nothing with it

 



def main():
    si = SaveImage("192.168.1.67/video2.mjpg", "Admin", "9999")
    si.saveImage()

if __name__ == "__main__":
    main()
