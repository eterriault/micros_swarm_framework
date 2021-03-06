/**
Software License Agreement (BSD)
\file      app1.cpp
\authors Xuefeng Chang <changxuefengcn@163.com>
\copyright Copyright (c) 2016, the micROS Team, HPCL (National University of Defense Technology), All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of micROS Team, HPCL, nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "apps/app1.h"

namespace micros_swarm_framework{

    struct XY
    {
        float x;
        float y;
    };

    App1::App1(ros::NodeHandle node_handle):Application(node_handle)
    {
    }

    App1::~App1()
    {
    }

    void App1::init()
    {
        //set parameters
        delta = 3;
        epsilon = 10;
    }

    float App1::force_mag(float dist)
    {
        return -(epsilon/(dist+0.1)) *(pow(delta/(dist+0.1), 4) - pow(delta/(dist+0.1), 2));
    }

    XY App1::force_sum(micros_swarm_framework::NeighborBase n, XY &s)
    {
<<<<<<< HEAD
        micros_swarm_framework::Base l=getRobotBase();
        float xl=l.getX();
        float yl=l.getY();

        float xn=n.getX();
        float yn=n.getY();

=======
        micros_swarm_framework::Base l=base();
        float xl=l.x;
        float yl=l.y;

        float xn=n.x;
        float yn=n.y;

        float dist=sqrt(pow((xl-xn),2)+pow((yl-yn),2));

        float fm = force_mag(dist)/1000;
        if(fm>0.5) fm=0.5;

        float fx=(fm/(dist+0.1))*(xn-xl);
        float fy=(fm/(dist+0.1))*(yn-yl);

        s.x+=fx;
        s.y+=fy;
        return s;
    }

    XY App1::direction()
    {
        XY sum;
        sum.x=0;
        sum.y=0;

        micros_swarm_framework::Neighbors<micros_swarm_framework::NeighborBase> n(true);
        boost::function<XY(NeighborBase, XY &)> bf=boost::bind(&App1::force_sum, this, _1, _2);

        sum=n.neighborsReduce(bf, sum);

        sum=n.reduce(bf, sum);

        return sum;
    }

    void App1::publish_cmd(const ros::TimerEvent&)
    {
        XY v=direction();

        trajectory_msgs::MultiDOFJointTrajectory trajectory_msg;
        trajectory_msg.header.stamp = ros::Time::now();

        mav_msgs::EigenTrajectoryPoint trajectory_point;

        micros_swarm_framework::Base l = getRobotBase();

        trajectory_point.position_W.x() = l.getX();
        trajectory_point.position_W.y() = l.getY();
        trajectory_point.position_W.z() = 1.0;


        trajectory_point.velocity_W.x() = v.x;
        trajectory_point.velocity_W.y() = v.y;

        mav_msgs::msgMultiDofJointTrajectoryFromEigen(trajectory_point, &trajectory_msg);


        pub_.publish(trajectory_msg);

    }

    void App1::motion()
    {
        timer = nh.createTimer(ros::Duration(0.1), &App1::publish_cmd, this);
    }

    void App1::baseCallback(const nav_msgs::Odometry& lmsg)
    {
        float x=lmsg.pose.pose.position.x;
        float y=lmsg.pose.pose.position.y;

        float vx=lmsg.twist.twist.linear.x;
        float vy=lmsg.twist.twist.linear.y;

        micros_swarm_framework::Base l(x, y, 0, vx, vy, 0);
        set_base(l);
    }

    void App1::start()
    {
        init();

        pub_ = nh_.advertise<trajectory_msgs::MultiDOFJointTrajectory>("command/trajectory", 1000);

        sub_ = nh_.subscribe("ground_truth/odometry", 1000, &App1::baseCallback, this, ros::TransportHints().udp());
        ros::Duration(5).sleep();  //this is necessary, in order that the runtime platform kernel of the robot has enough time to publish it's base information.

        motion();
    }
};
