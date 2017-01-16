#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/JointState.h>
#include "anrobot_description/FwdKinematics.h"

typedef struct kinematicsManagmentStruct
{
    bool use_kdl;
    ros::Publisher *marker_pub;
    ros::ServiceClient *kdl_client;
    ros::ServiceClient *simple_client;
} KinStruct;

void joint_states_cb(const sensor_msgs::JointStateConstPtr &msg, KinStruct k)
{
    anrobot_description::FwdKinematics srv;
    srv.request.theta1 = msg->position[0];
    srv.request.theta2 = msg->position[1];
    srv.request.d3 = msg->position[2];
    srv.request.use_kdl = k.use_kdl;

    visualization_msgs::Marker marker;
    marker.header.frame_id = "/base_link";
    marker.header.stamp = ros::Time::now();

    marker.ns = "end_pose";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;

    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;

    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

        if(srv.request.use_kdl)
        {
            if(k.kdl_client->call(srv))
            {
                marker.pose = srv.response.end_pose;
            }
        }
        else
        {
            if(k.simple_client->call(srv))
            {
                marker.pose = srv.response.end_pose;
            }
        }

       

        k.marker_pub->publish(marker);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "przez_kdl");
    

    ros::NodeHandle n;

    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::ServiceClient kdl_client = n.serviceClient<anrobot_description::FwdKinematics>("kdl_fwd");
  

    KinStruct k;
    k.use_kdl = 1;
    k.marker_pub = &marker_pub;
    k.kdl_client = &kdl_client;
    
    ros::Subscriber get_joint_states = n.subscribe<sensor_msgs::JointState>("joint_states", 100, boost::bind(joint_states_cb, _1, k));
    
    ros::spin();

    return 0;
}
