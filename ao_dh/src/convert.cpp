#include <ros/ros.h>
#include <iostream>
#include <kdl/kdl.hpp>
#include <kdl/tree.hpp>
#include <kdl/chain.hpp>
#include <math.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	KDL::Chain chain;	
	double pi = M_PI;
	double r, p, y;
	double joint[3];
	double a1 = 1, a2 = 3, alpha = pi, theta1 = pi/5, theta2 = pi/3, d3 =1;
	

	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame::DH(a1, 0, 0, theta1)));
	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame::DH(a2, alpha, 0, theta2)));
	chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame::DH(0, 0, d3, 0)));

	for (int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; j++){ 
			joint[j] = chain.getSegment(i).getFrameToTip().p.data[j];
		}
		chain.getSegment(i).getFrameToTip().M.GetRPY(r, p, y);
		
		if (i==1){
			std::cout << "Joint:" << i+1 << std::endl;
			std::cout << "xyz: " << joint[0] << ", " << joint[1] << ", " << joint[2] << std::endl;
			std::cout << "rpy: " << r << ", " << p << ", " << y << std::endl;
		}
		else{
			std::cout << "Joint:" << i+1 << std::endl;
			std::cout << "xyz: " << joint[0] << ", " << joint[1] << ", " << joint[2] << std::endl;
			std::cout << "rpy: " << r << ", " << p << ", " << y << std::endl;
		}
	}

	return 0;
}
