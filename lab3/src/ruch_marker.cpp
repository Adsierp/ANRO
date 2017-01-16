#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>


int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  geometry_msgs::PoseStamped osie;
  double steps[3];
  double a=0;

  osie.header.stamp = ros::Time::now();
  osie.header.frame_id = "map";
  osie.pose.position.x=1;
  osie.pose.position.y=1;
  osie.pose.position.z=1;
  
  ros::Publisher marker_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);
  marker_pub.publish(osie);
  while(ros::ok())
  {
	for(int i=0; i<10; i++){
		a=1;
		osie.header.frame_id = "map";
		osie.pose.position.x=a;
  		osie.pose.position.y=a;
  		osie.pose.position.z=a;	
		osie.header.stamp = ros::Time::now();
		marker_pub.publish(osie);
		ros::spinOnce();
		ros::Duration(1).sleep();
	}
  }
  
}
