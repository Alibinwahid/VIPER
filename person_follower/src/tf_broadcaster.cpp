#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Point.h>

std::string turtle_name = "turtle1";

void poseCallback(const geometry_msgs::Point& msg){
  static tf2_ros::TransformBroadcaster br;
  geometry_msgs::TransformStamped transformStamped;
  transformStamped.header.stamp = ros::Time::now();
  transformStamped.header.frame_id = "viper/left_camera_link";
  transformStamped.child_frame_id = "target";
  transformStamped.transform.translation.x = msg.z;
  transformStamped.transform.translation.y = -msg.x;
  transformStamped.transform.translation.z = 0;
  tf2::Quaternion q;
  q.setRPY(0, 0, 0);
  transformStamped.transform.rotation.x = q.x();
  transformStamped.transform.rotation.y = q.y();
  transformStamped.transform.rotation.z = q.z();
  transformStamped.transform.rotation.w = q.w();
  ROS_INFO("x = %f, y = %f", transformStamped.transform.translation.x, 
                             transformStamped.transform.translation.y);

  br.sendTransform(transformStamped);
}


int main(int argc, char** argv){
  ros::init(argc, argv, "target_tf2_broadcaster");

 /* ros::NodeHandle private_node("~");
  if (! private_node.hasParam("turtle"))
  {
    if (argc != 2){ROS_ERROR("need turtle name as argument"); return -1;};
    turtle_name = argv[1];
  }
  else
  {
    private_node.getParam("turtle", turtle_name);
  }
 */   
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/viper/follow_me/target_position", 10, &poseCallback);

  ros::spin();
  return 0;
};
