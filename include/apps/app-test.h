/*
 * app-test.h
 *
 *  Created on: 2016-09-01
 *      Author: evaterriault
 */

#ifndef APP_TEST_H_
#define APP_TEST_H_

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include <ctime>

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include <std_msgs/Float32.h>

#include "micros_swarm_framework/micros_swarm_framework.h"

namespace micros_swarm_framework{

	struct XY;

    class AppTest : public Application
    {
        public:
            ros::Timer timer_;
            ros::Publisher pub_;
            ros::Subscriber sub_;

            double interval;

            AppTest(ros::NodeHandle nh);
            ~AppTest();
            virtual void start();

            //app functions
            void init();
            void publish_cmd(const ros::TimerEvent&);
            void motion();
            void baseCallback(const nav_msgs::Odometry& lmsg);
            XY direction();
    };
};


#endif /* APP_TEST_H_ */
