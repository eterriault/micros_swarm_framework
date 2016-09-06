/*
 * app-test.cpp
 *
 *  Created on: 2016-09-01
 *      Author: evaterriault
 */


#include "apps/app-test.h"


using namespace std;

namespace micros_swarm_framework{

	struct XY
    {
        float x;
        float y;
    };

	AppTest::AppTest(ros::NodeHandle nh):Application(nh)
	    {

	    }

	AppTest::~AppTest(){}

	void AppTest::init()
	{
		interval = 1.0/10.0;
	}

	XY AppTest::direction(){

		//dumb direction based on robot ID
		XY sum;
		sum.x = getRobotID();
		sum.y = getRobotID();

//		micros_swarm_framework::Neighbors<micros_swarm_framework::NeighborBase> n(true);
//		boost::function<XY(NeighborBase, XY &)> bf=boost::bind(&AppTest::force_sum, this, _1, _2);
//		sum=n.neighborsReduce(bf, sum);

		return sum;
	}

	void AppTest::publish_cmd(const ros::TimerEvent&)
	{
		XY v=direction();
		std_msgs::Float32 t;
		t.data = v.x;

		pub_.publish(t);
	}

	void AppTest::motion()
	{
	   timer_ = nh_.createTimer(ros::Duration(0.1), &AppTest::publish_cmd, this);
	}

	void AppTest::baseCallback(const nav_msgs::Odometry& lmsg)
	{
	   float x=lmsg.pose.pose.position.x;
	   float y=lmsg.pose.pose.position.y;

	   float vx=lmsg.twist.twist.linear.x;
	   float vy=lmsg.twist.twist.linear.y;

	   micros_swarm_framework::Base l(x, y, 0, vx, vy, 0);
	   setRobotBase(l);
	}

	void AppTest::start()
	 {
	    init();

	    sub_ = nh_.subscribe("base_pose_ground_truth", 1000, &AppTest::baseCallback, this, ros::TransportHints().udp());
	    pub_ = nh_.advertise<std_msgs::Float32>("cmd_vel", 1000);

	    motion();
	}
}
