#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>



int main(int argc, char** argv){
  ros::init(argc, argv, "tf_listener");

  ros::NodeHandle node;

 /* ros::service::waitForService("spawn");
  ros::ServiceClient spawner =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn turtle;
  turtle.request.x = 4;
  turtle.request.y = 2;
  turtle.request.theta = 0;
  turtle.request.name = "turtle2";
  spawner.call(turtle);*/
  //ros::Publisher e100_vel = node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
  ros::Publisher distance_setpoint = node.advertise<std_msgs::Float64>("distance/setpoint", 10);
  ros::Publisher distance_state =  node.advertise<std_msgs::Float64>("distance/state", 10);
  ros::Publisher pid_enable =  node.advertise<std_msgs::Bool>("/pid_enable", 10);
  ros::Publisher angle_setpoint = node.advertise<std_msgs::Float64>("angle/setpoint", 10);
  ros::Publisher angle_state =  node.advertise<std_msgs::Float64>("angle/state", 10);
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  ros::Rate rate(10.0);
  while (node.ok()){
    geometry_msgs::TransformStamped transformStamped;
    try{
        transformStamped = tfBuffer.lookupTransform("viper/cvm_base_link", "target",
                               ros::Time(0));
      //transformStamped = tfBuffer.lookupTransform("turtle2", "turtle1",
      //                         ros::Time(0));

    }
    catch (tf2::TransformException &ex) {
      ROS_WARN("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    geometry_msgs::Twist vel_msg;
    double angle = atan2(transformStamped.transform.translation.y, transformStamped.transform.translation.x);
    double distance = transformStamped.transform.translation.x;
    ROS_INFO("x = %f, phi = %f \n", distance, angle);
    std_msgs::Float64 __setpoint; std_msgs::Float64 __state; 
    std_msgs::Bool __pid_enable;
    __pid_enable.data = 1;
    pid_enable.publish(__pid_enable);
//===================Distance Loop================================
    __setpoint.data = 1.5; 
    __state.data = distance; 
    distance_setpoint.publish(__setpoint);
    distance_state.publish(__state);
//===================Angle Loop================================
    __setpoint.data = 0; 
    __state.data = angle; 
    angle_setpoint.publish(__setpoint);
    angle_state.publish(__state);
    

//    e100_vel.publish(vel_msg);
    rate.sleep();
  }
  return 0;
};