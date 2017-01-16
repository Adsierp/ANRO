#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <termios.h>

int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "turtlesim_control");
	ros::NodeHandle n;
	geometry_msgs::Twist con_msg;
	int znak;

	ros::Publisher control_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
	ros::Rate loop_rate(1);

	while(ros::ok())
	{
		znak=getch();
		if (znak == 'w')
		{
			con_msg.linear.x = 2.0;
	   		con_msg.linear.y = 0.0;
	  		con_msg.linear.z = 0.0;
	
			con_msg.angular.x = 0.0;
			con_msg.angular.y = 0.0;
			con_msg.angular.z = 0.0;
		}
		else if	(znak == 's')
		{
			con_msg.linear.x = -2.0;
	   		con_msg.linear.y = 0.0;
	  		con_msg.linear.z = 0.0;
	
			con_msg.angular.x = 0.0;
			con_msg.angular.y = 0.0;
			con_msg.angular.z = 0.0;
		}
		else if	(znak == 'a')
		{
			con_msg.linear.x = 0.0;
	   		con_msg.linear.y = 0.0;
	  		con_msg.linear.z = 0.0;
	
			con_msg.angular.x = 0.0;
			con_msg.angular.y = 0.0;
			con_msg.angular.z = 2.0;
		}
		else if	(znak == 'd')
		{
			con_msg.linear.x = 0.0;
	   		con_msg.linear.y = 0.0;
	  		con_msg.linear.z = 0.0;
	
			con_msg.angular.x = 0.0;
			con_msg.angular.y = 0.0;
			con_msg.angular.z = -2.0;
		}		
		else
		{
			con_msg.linear.x = 0.0;
	   		con_msg.linear.y = 0.0;
	  		con_msg.linear.z = 0.0;
	
			con_msg.angular.x = 0.0;
			con_msg.angular.y = 0.0;
			con_msg.angular.z = 0.0;
		}
		control_pub.publish(con_msg);
		ROS_INFO("Moved %c", znak);
		ros::spinOnce();
		loop_rate.sleep();

	}

	

   return 0;
}
