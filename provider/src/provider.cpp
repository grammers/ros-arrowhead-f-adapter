
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h"
#include "../../comon/messages.hpp"
#include "arrowhead/arrowhead_cpp.h"

#include <sstream>
#include <string>
#include <stdio.h>
#include <thread>
#include <list>
#include <time.h>
#include <iomanip>

#ifdef __linux__
     #include <unistd.h>
#elif _WIN32
     #include <windows.h>
#endif

using namespace arrowhead;
// create instances of classes that are used
Converter convert;
Arrowhead _arrowhead;


int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "provider");
	ros::NodeHandle n;

	// crate a ROS subscriber that listens for a temperature measurement
	ros::Publisher temperature_pub = n.advertise<sensor_msgs::Temperature>("temperature_provider", 10);

	// prams setted in launch
	// stored in a _arrowhead struck named Arrowhead_Data_ext
	// mainly used to configure the precumer
	ros::NodeHandle nh("~");
	nh.param<std::string>("ACCESS_URI", _arrowhead.config.ACCESS_URI, "http://_arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("ACCESS_URI_HTTPS", _arrowhead.config.ACCESS_URI_HTTPS, "https://_arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("THIS_ADDRESS", _arrowhead.config.THIS_ADDRESS, "10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", _arrowhead.config.THIS_ADDRESS6, "[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("SERVICE_URI", _arrowhead.config.SERVICE_URI, "this_is_the_custom_url");
	nh.param<std::string>("THIS_SYSTEM_NAME", _arrowhead.config.THIS_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", _arrowhead.config.SERVICE_DEFINITION, "IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", _arrowhead.config.INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", _arrowhead.config.PRIVATE_KEY_PATH, "keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", _arrowhead.config.PUBLIC_KEY_PATH, "keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", _arrowhead.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", _arrowhead.config.SECURITY, "token");
	nh.param<int>("THIS_PORT", _arrowhead.config.THIS_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", _arrowhead.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", _arrowhead.config.SECURE_PROVIDER_INTERFACE, false);
	
	// Print the configure parameters
	_arrowhead.config.print();
	
	// Set up the _arrowhead part
	// param is the baseName, used to verify that the correct messages are sent. 
	// a pointer to the callback function for POST msgs
	_arrowhead.init_provider(_arrowhead.config.THIS_SYSTEM_NAME);
	printf("it is init");

	// Set up the message
	// param 
	// @ sensor_id identification name (suggest, use same as service id in _arrowhead database)
	// @ unit the unit that the data are sent in
	// @ baseName must be some as above
	convert.init("sensor_id", _arrowhead.config.UNIT, _arrowhead.config.THIS_SYSTEM_NAME);



	// to reserve msgs mast it be define first
	// should not be don her. 
	// But good for testing
	// TODO set it to whit for first update form sensor
	convert.set(404, 1);
	ros::Time t(0,0);
	convert.temperature.header.stamp = t;
    _arrowhead.setProviderMsgs(convert.getJsonMsgs());
	
	// start running
	// publish latest data every 5s
	ros::Rate loop_sleep(0.2);
	while (ros::ok()) {
		temperature_pub.publish(convert.temperature);
		ros::spinOnce();
		loop_sleep.sleep();
	}

	return 0;
}

