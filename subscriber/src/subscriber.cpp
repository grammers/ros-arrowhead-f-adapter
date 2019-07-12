
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h"
#include "../../comon/messages.hpp"
#include "arrowhead/Subscriber.h"

#include <sstream>
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
Subscriber subscriber;


int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "subscriber");
	ros::NodeHandle n;

	// crate a ROS subscriber that listens for a temperature measurement
	ros::Publisher temperature_pub = n.advertise<sensor_msgs::Temperature>(
					"temperature_subscriber", 10);

	// prams setted in launch
	// stored in a arrowhead struck named Arrowhead_Data_ext
	// mainly used to configure the precumer
	ros::NodeHandle nh("~");
	nh.param<std::string>("ACCESS_URI", subscriber.config.ACCESS_URI, 
					"http://arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("ACCESS_URI_HTTPS", 
					subscriber.config.ACCESS_URI_HTTPS, 
					"https://arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("THIS_ADDRESS", subscriber.config.THIS_ADDRESS, 
					"10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", subscriber.config.THIS_ADDRESS6, 
					"[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("SERVICE_URI", subscriber.config.SERVICE_URI, 
					"this_is_the_custom_url");
	nh.param<std::string>("THIS_SYSTEM_NAME", subscriber.config.THIS_SYSTEM_NAME, 
					"SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", subscriber.config.SERVICE_DEFINITION, 
					"IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", subscriber.config.INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", subscriber.config.PRIVATE_KEY_PATH, 
					"keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", subscriber.config.PUBLIC_KEY_PATH, 
					"keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", subscriber.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", subscriber.config.SECURITY, "token");
	nh.param<int>("THIS_PORT", subscriber.config.THIS_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", 
					subscriber.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", 
					subscriber.config.SECURE_PROVIDER_INTERFACE, false);
	
	// Print the configure parameters
	subscriber.config.print();
	
	// Set up the arrowhead part
	// param is the baseName, used to verify that the correct messages are sent. 
	// a pointer to the callback function for POST msgs
	subscriber.init(subscriber.config.THIS_SYSTEM_NAME, Converter::parce);
	
	// to reserve msgs mast it be define first
	// should not be don her. 
	// But good for testing
	// TODO set it to whit for first update form sensor
	convert.set(404, 1);
    
	// start running
	// publish latest data every 5s
	ros::Rate loop_sleep(0.2);
	while (ros::ok()) {
		ros::spinOnce();
		loop_sleep.sleep();
		temperature_pub.publish(convert.temperature);
	}

	return 0;
}

