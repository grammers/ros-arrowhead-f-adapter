
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h"
#include "messages.hpp"
#include "Provider/SensorHandler.h"

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

// create instances of classes that are used
Converter convert;
SensorHandler oSensorHandler;


int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "subscriber_example");
	ros::NodeHandle n;

	// crate a ROS subscriber that listens for a temperature measurement
	ros::Publisher temperature_pub = n.advertise<sensor_msgs::Temperature>("temperature_example", 10);

	// prams setted in launch
	// stored in a arrowhead struck named Arrowhead_Data_ext
	// mainly used to configure the precumer
	ros::NodeHandle nh("~");
	nh.param<std::string>("SR_BASE_URI", oSensorHandler.config.SR_BASE_URI, "http://arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("SR_BASE_URI_HTTPS", oSensorHandler.config.SR_BASE_URI_HTTPS, "https://arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("THIS_ADDRESS", oSensorHandler.config.THIS_ADDRESS, "10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", oSensorHandler.config.THIS_ADDRESS6, "[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("CUSTOM_URL", oSensorHandler.config.CUSTOM_URL, "this_is_the_custom_url");
	nh.param<std::string>("THIS_SYSTEM_NAME", oSensorHandler.config.THIS_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", oSensorHandler.config.SERVICE_DEFINITION, "IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", oSensorHandler.config.INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", oSensorHandler.config.PRIVATE_KEY_PATH, "keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", oSensorHandler.config.PUBLIC_KEY_PATH, "keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", oSensorHandler.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", oSensorHandler.config.SECURITY, "token");
	nh.param<int>("THIS_PORT", oSensorHandler.config.THIS_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", oSensorHandler.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_THIS_INTERFACE", oSensorHandler.config.SECURE_THIS_INTERFACE, false);
	
	// Print the configure parameters
	oSensorHandler.config.print();
	
	// Set up the arrowhead part
	// param is the baseName, used to verify that the correct messages are sent. 
	// a pointer to the callback function for POST msgs
	oSensorHandler.initSensorHandler(oSensorHandler.config.THIS_SYSTEM_NAME, Converter::parce);
	
	// Set up the message
	// param 
	// @ sensor_id identification name (suggest, use same as service id in arrowhead database)
	// @ unit the unit that the data are sent in
	// @ baseName must be some as above
	convert.init("sensor_id", oSensorHandler.config.UNIT, oSensorHandler.config.THIS_SYSTEM_NAME);



	// to reserve msgs mast it be define first
	// should not be don her. 
	// But good for testing
	// TODO set it to whit for first update form sensor
	convert.set(404, 1);
	ros::Time t(0,0);
	convert.temperature.header.stamp = t;
    oSensorHandler.processProvider(convert.getJsonMsgs());
	
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

