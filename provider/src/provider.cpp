
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h" 
#include "../../comon/messages.hpp" 
#include "arrowhead/Provider.h"
#include "arrowhead/Consumer.h"

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
Consumer real_data;
bool REAL; // use real sensor


// callback from real sensor
void callback(const char* url, const char* msgs) {
	// pars the msgs to ros msgs
	convert.pars(url, msgs);
}

int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "provider"); ros::NodeHandle n;

	// crate a ROS publisher that publisher the temperature measurement
	ros::Publisher temperature_pub = n.advertise<sensor_msgs::Temperature>(
					"temperature_provider", 10);

	// prams setted in launch stored in a provider struck named
	// ArrowheadiDataExt mainly used to configure the precumer
	ros::NodeHandle nh("~"); nh.param<std::string>("ACCESS_URI",
					provider.config.ACCESS_URI,
					"http://provider.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("THIS_ADDRESS", provider.config.THIS_ADDRESS,
					"10.0.0.40");
	nh.param<std::string>("SERVICE_URI", provider.config.SERVICE_URI,
					"this_is_the_custom_url");
	nh.param<std::string>("THIS_SYSTEM_NAME",
					provider.config.THIS_SYSTEM_NAME,
					"SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_NAME",
					provider.config.SERVICE_NAME,
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
	nh.param<bool>("REAL", REAL, false);
	
	nh.param<std::string>("OR_URI", real_data.config.ACCESS_URI, "a name");
	nh.param<std::string>("SERVICE_NAME_CONSUMER", real_data.config.SERVICE_NAME, "a name");
	nh.param<std::string>("TARGET_SYSTEM_NAME", real_data.config.TARGET_SYSTEM_NAME, "providers name");
	nh.param<std::string>("TARGET_ADDRESS", real_data.config.TARGET_ADDRESS, "ip to target");
	nh.param<int>("TARGET_PORT", real_data.config.TARGET_PORT, 1234);
	nh.param<bool>("OVERRIDE_STORE", real_data.config.OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", real_data.config.MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", real_data.config.METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", real_data.config.PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", real_data.config.ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", real_data.config.EXTERNAL_SERVICE_REQUEST, false);
	
	
	real_data.config.UNIT = provider.config.UNIT;
	real_data.config.THIS_ADDRESS = provider.config.THIS_ADDRESS;
	real_data.config.THIS_PORT= 8498;
	real_data.config.THIS_SYSTEM_NAME = "real_resever";
	real_data.config.INTERFACE = provider.config.INTERFACE;
	real_data.config.SECURITY = provider.config.SECURITY;
	real_data.config.SECURE_PROVIDER_INTERFACE = provider.config.SECURE_PROVIDER_INTERFACE;
	real_data.config.SECURE_ARROWHEAD_INTERFACE = provider.config.SECURE_ARROWHEAD_INTERFACE;

	// Print the configure parameters
	provider.config.print();
	printf("\n");
	if(REAL) real_data.config.print();

	// update rate 5s (inaccurate)
	ros::Rate loop_sleep(0.2);

	// loop to eliminate race condition
	if(REAL){
		while(!real_data.init(callback)){
			fprintf(stderr, "retry connecting to real sensor in a moment\n");
			loop_sleep.sleep();
		}
	}

	
	// init the provider now when the parameters are set.
	provider.init();
	printf("The provider it is init\n");

	// Set up the message 
	// param @ sensor_id identification name 
	// (suggest, use same as service id in provider database) 
	// @ unit the unit that the data are sent in 
	// @ serviseName must be some as above
	convert.init("sensor_id", provider.config.UNIT,
					provider.config.SERVICE_NAME);

	convert.temperature.temperature = 0;
	convert.temperature.header.stamp.sec = std::time(0);
	
	double t; // demo counter instead of real sensor

	while (ros::ok()) {
		// convert current ros msgs to json msgs
		convert.updateMsgs();
		// update the messages that will be return on request
		provider.setMsgs(convert.getJsonMsgs());

		if(REAL){
			// send request to real sensor
			real_data.request();
		}
		else {
			// set new temperature demo
			convert.temperature.temperature = t;
			convert.temperature.header.stamp.sec = std::time(0);
			t++;
		}

		// ROS publish the temperature
		temperature_pub.publish(convert.temperature); 

		// sleep until next
		ros::spinOnce();
		loop_sleep.sleep(); 
	}

	return 0;
}

