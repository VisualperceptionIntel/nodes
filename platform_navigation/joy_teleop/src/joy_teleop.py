#!/usr/bin/env python
"""************************************************************************************************
***                                                                                             ***
***                                      ###############                                        ***
***                              ##     #################        ##                             ***
***                         #######    ####################      #######                         ***
***                     ##########     #####################      ##########                    ***
***                  ############     #######      ##########     ############                  ***
***           ####   ############     ######          ########     ############   ####          ***
***      ########   #############     ######           #######     #############   ########     ***
***   ##########    #############     ######            ######     #############   ##########   ***
***   ##########    #############     ######            ######     #############   ##########   ***
***      ########   #############     ######           #######     #############   ########     ***
***           ####   ############     ######          ########     ############   ####          ***
***                   ############    #######       #########     ############                  ***
***                     ##########     #####################      ##########                    ***
***                         #######    ###################       #######                         ***
***                              ##     ################         ##                             ***
***                                      #############                                          ***
***                   ____  ___  ____   ____   ___ __     __ _____  ____   ____                 ***
***                  |  _ \|_ _||  _ \ |  _ \ |_ _|\ \   / /| ____||  _ \ / ___|                ***
***                  | | | || | | | | || |_) | | |  \ \ / / |  _|  | |_) |\___ \                ***
***                  | |_| || | | |_| ||  _ <  | |   \ V /  | |___ |  _ <  ___) |               ***
***                  |____/|___||____/ |_| \_\|___|   \_/   |_____||_| \_\|____/                ***
***                                                                                             ***
***             ACCELERATING THE WORLDS TRANSITION TO AUTONOMOUS VEHICLES AND ROBOTICS          ***
***                                                                                             ***
***************************************************************************************************
*
*  File: drive_controller_node.py
*  Desc: This node subscribe to the nav_mode topic and enables the corresponding subscribers to get velocity commands:
*     IF nav_mode is:
*       AUTO      : subscribe to cmd_vel & write to motor controller
*       MANUAL    : subscribe to /joystick/ui/cmd_vel & write to motor controller
*       REMOTE    : subscribe to /joystick/controller/cmd_vel & write to motor controller
*       all other : write the velocity command [0,0] to motor controller
*        
*  Maintainers: Mahindan
*               
*
*  Subscribes:
*      - /nav_mode
*      - /cmd_vel
*      - /joystick/ui/cmd_vel
*      - /joystick/controller/cmd_vel
*
*  Publishes:
*      - //status of the write to motor controller should be publish onto a feedback topic 
*
*  Serial read:
*      - //should read the status of the motor controller after writing
*
*  Serial write:
*      -  "x:'value',yaw:'value'\r"
*                                                                                                 *
*  Copyright (c) 2017, DiDrivers Limited.                                                         *
*  All Rights Reserved                                                                            *
*                                                                                                 *
*  Redistribution and use in source and binary forms, with or without                             *
*  modification, are permitted provided that the following conditions are met:                    *
*      * Redistributions of source code must retain the above copyright                           *
*        notice, this list of conditions and the following disclaimer.                            *
*      * Redistributions in binary form must reproduce the above copyright                        *
*        notice, this list of conditions and the following disclaimer in the                      *
*        documentation and/or other materials provided with the distribution.                     *
*      * Neither the name of DiDrivers Limited. nor the                                           *
*        names of its contributors may be used to endorse or promote products                     *
*        derived from this software without specific prior written permission.                    *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL Didrivers Limited. BE LIABLE FOR ANY                              *
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                      *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Please send comments, questions, or patches to support@didrivers.com                            *
************************************************************************************************"""
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
import time
from sensor_msgs.msg import Joy
#import serial.tools.list_ports
#controller = serial.Serial
pub = rospy.Publisher('joystick/controller/cmd_vel', Twist, queue_size=1)
pub_navMode = rospy.Publisher('nav_mode', String, queue_size=1)
configMode = False
joy_tractorMode = False
navMode = String()
navMode.data = 'AUTO'
configTime = 0
LBindex = 4
RBindex = 5
Xindex = 2
Oindex = 2
Bindex = 1
Aindex = 0
BACKindex = 6

AXES_RT = 5
AXES_LT = 2

def joyCmd_cb(joyCmd_msg):
	rate = rospy.Rate(20)
	global configMode
	global joy_tractorMode	
	global configTime
	global navMode

	now = rospy.get_rostime()
	if(joyCmd_msg.buttons[LBindex] == 1 and joyCmd_msg.buttons[RBindex] ==1 and joyCmd_msg.buttons[Bindex] ==1):
		configMode = True
		configTime = rospy.get_rostime()
	if configMode and now < configTime + rospy.Duration(10):
		if(joyCmd_msg.buttons[LBindex] == 0 and joyCmd_msg.buttons[RBindex] ==0 and joyCmd_msg.buttons[Bindex] ==0 and joyCmd_msg.buttons[BACKindex] ==1):
			joy_tractorMode = False
			configMode = False
		if(joyCmd_msg.buttons[LBindex] == 0 and joyCmd_msg.buttons[RBindex] ==0 and joyCmd_msg.buttons[Bindex] ==0 and joyCmd_msg.buttons[Xindex] ==1):
			joy_tractorMode = True
			configMode = False
		if(joyCmd_msg.buttons[LBindex] == 0 and joyCmd_msg.buttons[RBindex] ==0 and joyCmd_msg.buttons[Bindex] ==0 and joyCmd_msg.axes[AXES_LT] < 0 and joyCmd_msg.buttons[Aindex] ==1):
			navMode.data = 'AUTO'
			pub_navMode.publish(navMode)
			joy_tractorMode = False
			configMode = False
		if(joyCmd_msg.buttons[LBindex] == 0 and joyCmd_msg.buttons[RBindex] ==0 and joyCmd_msg.buttons[Bindex] ==0 and joyCmd_msg.axes[AXES_LT] < 0 and joyCmd_msg.buttons[Xindex] ==1):
			navMode.data = 'REMOTE'
			pub_navMode.publish(navMode)
			joy_tractorMode = True
			configMode = False

	else:
		configMode = False
		
	if joy_tractorMode:
		twistMsg = Twist()
		twistMsg.linear.x = float("%.2f" % joyCmd_msg.axes[1])
		twistMsg.angular.z =float("%.2f" % joyCmd_msg.axes[3])
		pub.publish(twistMsg)
		rate.sleep()
	
	# send the string to the device
	
	
	
												
def joy_teleop():
	rospy.init_node('joy_teleop')
	rospy.Subscriber("joy", Joy, joyCmd_cb,queue_size=5)
	rospy.spin()
	#controller.close()
												

if __name__ == '__main__':
    try:
        joy_teleop()

    except rospy.ROSInterruptException:
	pass

