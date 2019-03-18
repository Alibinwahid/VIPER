#!/usr/bin/env python  
import roslib
roslib.load_manifest('learning_tf')
import rospy
import math
import tf
import geometry_msgs.msg
import turtlesim.srv
from std_msgs.msg import Float64
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Point
import numpy as np

cmd = geometry_msgs.msg.Twist()
cmd_vel = rospy.Publisher('cmd_vel', geometry_msgs.msg.Twist,queue_size=1)
safety_distance = 0.8
safety_warning = False

def controller_effort_distance(msg):
    cmd.linear.x = msg.data/7

def controller_effort_angle(msg):
    cmd.angular.z = msg.data/7

def pose_of_person(msg):
    print ("Inside loop")
    cmd_vel.publish(cmd)
def collision_avoidance(msg):
    global safety_warning
    laser = np.array(msg.ranges[200:550])
    warning_counts = 0
    for i in laser:
        if i < safety_distance:
            warning_counts += 1
    if warning_counts > 10:
        safety_warning = True
        print ("WARNING!! Collision Ahead!!!!")
    elif warning_counts < 10:
        safety_warning = False
        print ("All clear!!")

        
    
if __name__ == '__main__':
    try:
        rospy.init_node('drive_rabbot')
        rospy.Subscriber('/angle/control_effort', Float64, controller_effort_angle)
        rospy.Subscriber('/distance/control_effort', Float64, controller_effort_distance)
        rospy.Subscriber('/scan', LaserScan, collision_avoidance)
        rospy.Subscriber('/viper/follow_me/target_position', Point, pose_of_person)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
