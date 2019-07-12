
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h" 
#include "../../comon/messages.hpp" 
#include "arrowhead/Provider.h"

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
Provider provider;


int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "provider"); ros::NodeHandle n;

	// crate a ROS subscriber that listens for a temperature measurement
	ros::Publisher temperature_pub = n.advertise<sensor_msgs::Temperature>(
					"temperature_provider", 10);

	// prams setted in launch stored in a provider struck named
	// Arrowhead_Data_ext mainly used to configure the precumer
	ros::NodeHandle nh("~"); nh.param<std::string>("ACCESS_URI",
					provider.config.ACCESS_URI,
					"http://provider.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("ACCESS_URI_HTTPS",
					provider.config.ACCESS_URI_HTTPS,
					"https://provider.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("THIS_ADDRESS", provider.config.THIS_ADDRESS,
					"10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", provider.config.THIS_ADDRESS6,
							"[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("SERVICE_URI", provider.config.SERVICE_URI,
					"this_is_the_custom_url");
	nh.param<std::string>("THIS_SYSTEM_NAME",
					provider.config.THIS_SYSTEM_NAME,
					"SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION",
					provider.config.SERVICE_DEFINITION,
					"IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", provider.config.INTERFACE,
					"JSON"); nh.param<std::string>("PRIVATE_KEY_PATH",
							provider.config.PRIVATE_KEY_PATH,
							"keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH",
					provider.config.PUBLIC_KEY_PATH,
					"keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", provider.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", provider.config.SECURITY, "token");
	nh.param<int>("THIS_PORT", provider.config.THIS_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE",
					provider.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE",
					provider.config.SECURE_PROVIDER_INTERFACE, false);
	
	// Print the configure parameters
	provider.config.print();
	
	// Set up the provider part param is the baseName, used to verify
	// that the correct messages are sent. 
	// a pointer to the callback
	// function for POST msgs
	provider.init(provider.config.THIS_SYSTEM_NAME);
	printf("it is init");

	// Set up the message 
	// param @ sensor_id identification name 
	// (suggest, use same as service id in provider database) 
	// @ unit the unit that the data are sent in 
	// @ baseName must be some as above
	convert.init("sensor_id", provider.config.UNIT,
					provider.config.THIS_SYSTEM_NAME);

	
	double t;
	// start running publish latest data every 5s
	ros::Rate loop_sleep(0.2);
	while (ros::ok()) {

		Converter::temperature.temperature = t;
		Converter::temperature.header.stamp.sec = std::time(0);
		convert.updateMsgs();
		provider.setMsgs(convert.getJsonMsgs());

		temperature_pub.publish(convert.temperature); 
		ros::spinOnce();
		loop_sleep.sleep(); 
		t++;
	}

	return 0;
}

