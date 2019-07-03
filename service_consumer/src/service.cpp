
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

// The ROS callback function
// reads input
// set msgs to send
void getTempCallback(const sensor_msgs::Temperature::ConstPtr& msg){
	// set new msgs and update msgs to be sent
	convert.set(msg->temperature, msg->header.seq); 
    oSensorHandler.processProvider(convert.getJsonMsgs());
	return;
}

int main(int argc, char* argv[]){
	// init ROS
	ros::init(argc, argv, "service_example");
	ros::NodeHandle n;

	// crate a ROS subscriber that listens for a temperature measurement
	ros::Subscriber temperature_sub = n.subscribe<sensor_msgs::Temperature>("temperature_example", 10,  getTempCallback);

	// prams setted in launch
	// stored in a arrowhead struck named Arrowhead_Data_ext
	// mainly used to configure the precumer
	ros::NodeHandle nh("~");
	nh.param<std::string>("SR_BASE_URI", oSensorHandler.config.SR_BASE_URI, "http://arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("SR_BASE_URI_HTTPS", oSensorHandler.config.SR_BASE_URI_HTTPS, "https://arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("PROVIDER_ADDRESS", oSensorHandler.config.PROVIDER_ADDRESS, "10.0.0.40");
	nh.param<std::string>("PROVIDER_ADDRESS6", oSensorHandler.config.PROVIDER_ADDRESS6, "[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("CUSTOM_URL", oSensorHandler.config.CUSTOM_URL, "this_is_the_custom_url");
	nh.param<std::string>("PROVIDER_SYSTEM_NAME", oSensorHandler.config.PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", oSensorHandler.config.SERVICE_DEFINITION, "IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", oSensorHandler.config.INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", oSensorHandler.config.PRIVATE_KEY_PATH, "keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", oSensorHandler.config.PUBLIC_KEY_PATH, "keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", oSensorHandler.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", oSensorHandler.config.SECURITY, "token");
	nh.param<int>("PROVIDER_PORT", oSensorHandler.config.PROVIDER_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", oSensorHandler.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", oSensorHandler.config.SECURE_PROVIDER_INTERFACE, false);
	
	// Print the configure parameters
	oSensorHandler.config.print();
	
	// Set up the arrowhead part
	// param is the baseName, used to verify that the correct messages are sent. 
	oSensorHandler.initSensorHandler(oSensorHandler.config.PROVIDER_SYSTEM_NAME);
	
	// Set up the message
	// param 
	// @ sensor_id identification name (suggest, use same as service id in arrowhead database)
	// @ unit the unit that the data are sent in
	// @ baseName must be some as above
	convert.init("sensor_id", oSensorHandler.config.UNIT, oSensorHandler.config.PROVIDER_SYSTEM_NAME);



	// to reserve msgs mast it be define first
	// should not be don her. 
	// But good for testing
	// TODO set it to whit for first update form sensor
	convert.set(404, 1);
    oSensorHandler.processProvider(convert.getJsonMsgs());
	
	// start running
	// wait for interrupts
	while (ros::ok()) {
		ros::spin();
	}

	return 0;
}

