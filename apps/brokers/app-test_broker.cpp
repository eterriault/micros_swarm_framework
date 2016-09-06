/*
 * app-test_broker.cpp
 *
 *  Created on: 2016-09-01
 *      Author: evaterriault
 */


#include "apps/app-test.h"

namespace micros_swarm_framework{

    class AppTestBroker : public nodelet::Nodelet
    {
        public:
            ros::NodeHandle nh_;
            boost::shared_ptr<Application> app_;

            AppTestBroker();
            ~AppTestBroker();
            virtual void onInit();
    };

    AppTestBroker::AppTestBroker()
    {

    }

    AppTestBroker::~AppTestBroker()
    {

    }

    void AppTestBroker::onInit()
    {
        nh_ = getNodeHandle();
        app_=Singleton<AppTest>::getSingleton(nh_);
        app_->start();
    }
};

// Register the nodelet
PLUGINLIB_EXPORT_CLASS(micros_swarm_framework::AppTestBroker, nodelet::Nodelet)





