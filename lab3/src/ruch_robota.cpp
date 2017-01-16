#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/JointState.h>
#include <iostream>
using namespace std;


class Trajectory
{
    private:
        sensor_msgs::JointState start_pos, end_pos;
        double steps[3], accel[3];
        void init();

    public:
        int msg_amount;
        int inc; 
        Trajectory(ros::Publisher* pub);
        ros::Publisher* pub_ptr;
        sensor_msgs::JointState current;
        bool change(sensor_msgs::JointStateConstPtr input);
        void init_lin(sensor_msgs::JointStateConstPtr msg);
        void next_step(const ros::TimerEvent& event);
        void set_end_pos(sensor_msgs::JointStateConstPtr end_pos);

};

void Trajectory::init()
{
    msg_amount = 300;
    inc = 0;
    current.position.push_back(0);
    current.position.push_back(0);
    current.position.push_back(1.65);
    current.velocity.push_back(0);
    current.velocity.push_back(0);
    current.velocity.push_back(0);
    current.effort.push_back(0);
    current.effort.push_back(0);
    current.effort.push_back(0);
    current.name.push_back("joint1");
    current.name.push_back("joint2");
    current.name.push_back("joint3");
    start_pos = current;
    end_pos = current;
}

Trajectory::Trajectory(ros::Publisher* pub)
{
    init();
    pub_ptr = pub;
}

void Trajectory::next_step(const ros::TimerEvent& event)
{
    if (++inc < msg_amount)
    {
        current.position[0] += steps[0];
        current.position[1] += steps[1];
        current.position[2] += steps[2];
    }

    current.header.stamp = ros::Time::now();
    pub_ptr->publish(current);
}


void Trajectory::set_end_pos(sensor_msgs::JointStateConstPtr new_end_pos)
{
    this->end_pos = *new_end_pos;
}

void Trajectory::init_lin(sensor_msgs::JointStateConstPtr msg)
{
    inc = 0;
    start_pos = current;
    set_end_pos(msg);
    steps[0] = (end_pos.position[0]-start_pos.position[0])/msg_amount;
    steps[1] = (end_pos.position[1]-start_pos.position[1])/msg_amount;
    steps[2] = (end_pos.position[2]-start_pos.position[2])/msg_amount;
}

void end_pos_states_cb(const sensor_msgs::JointStateConstPtr &msg, ros::Timer *timer, Trajectory *t)
{
    if (t->change(msg))
    {
        t->init_lin(msg);
        timer->start();
    }
    else
        t->current.header.stamp = ros::Time::now();
        t->pub_ptr->publish(t->current);
}

bool Trajectory::change(sensor_msgs::JointStateConstPtr input)
{
    double diff = 0, sq;
    for(int i = 0; i < 3; ++i)
    {
        sq = (input->position[i] - end_pos.position[i]);
        diff += sq*sq;
    }
    if (diff > 0.01)
        return 1;
    else
        return 0; 
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "ruch_robota");
    ros::NodeHandle n;

    ros::Publisher trajectory_pub = n.advertise<sensor_msgs::JointState>("trajectory_joint_states", 1);

    Trajectory t(&trajectory_pub);
    ros::Timer timer = n.createTimer(ros::Duration(4./200), &Trajectory::next_step, &t, false, false);

    ros::Subscriber get_end_pose_state = n.subscribe<sensor_msgs::JointState>("joint_states", 100, boost::bind(end_pos_states_cb, _1, &timer, &t));

    while(ros::ok())
    {
        if(t.inc > t.msg_amount) timer.stop();
        ros::spinOnce();
    }

    return 0;
}
