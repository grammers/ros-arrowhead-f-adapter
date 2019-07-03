
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

// the messages that are sent
Converter convert;
SensorHandler oSensorHandler;

const std::string version = "4.0";


void getTempCallback(const sensor_msgs::Temperature::ConstPtr& msg){
    time_t linuxEpochTime = std::time(0);
	convert.set(msg->temperature, msg->header.seq);
    oSensorHandler.processProvider(convert.getJsonMsgs());
	return;
}

int main(int argc, char* argv[]){
	ros::init(argc, argv, "service_example");
	ros::NodeHandle n;

	ros::Subscriber temperature_sub = n.subscribe<sensor_msgs::Temperature>("temperature_example", 10,  getTempCallback);

	// prams
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
	
	
	oSensorHandler.config.print();
	
	printf("\n=============================\nProvider Example - v%s\n=============================\n", version.c_str());
	oSensorHandler.initSensorHandler(oSensorHandler.config.PROVIDER_SYSTEM_NAME);
	convert.init("sensor_id", oSensorHandler.config.UNIT, oSensorHandler.config.PROVIDER_SYSTEM_NAME);


//SenML format
//todo:
//generate own measured value into "measuredValue"
//"value" should be periodically updated
//"sLinuxEpoch" should be periodically updated


   	oSensorHandler.registerSensor();
	// to reserve msgs mast it be define first
	// should not be don her. 
	// But good for testing
	convert.set(404, 1);
    oSensorHandler.processProvider(convert.getJsonMsgs());
	
	while (ros::ok()) {
		ros::spin();
	}

	return 0;
}

