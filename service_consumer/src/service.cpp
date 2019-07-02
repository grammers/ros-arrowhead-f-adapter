
#pragma warning(disable:4996)

#include "ros/ros.h"
#include "sensor_msgs/Temperature.h"
#include "messages.hpp"
#include "Provider/SensorHandler.h"
#include "global_const.hpp"

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
Converter convert("Sensor_id", "Celsius", "100");
SensorHandler oSensorHandler;

// global constants
// do not temper whit these in code (unless you know wot you are doing)
// it is supposed to be defined as constants, but that is no compilable
std::string SR_BASE_URI;
std::string SR_BASE_URI_HTTPS;
std::string PROVIDER_ADDRESS;
std::string PROVIDER_ADDRESS6;
std::string CUSTOM_URL;
std::string PROVIDER_SYSTEM_NAME;
std::string SERVICE_DEFINITION;
std::string INTERFACE;
std::string PRIVATE_KEY_PATH;
std::string PUBLIC_KEY_PATH;
int PROVIDER_PORT;
bool SECURE_ARROWHEAD_INTERFACE;
bool SECURE_PROVIDER_INTERFACE;

void debug_print(){
	printf("\nDebug print for set prams\n");
	printf("SR_BASE_URI: %s\n", SR_BASE_URI.c_str());
	printf("SR_BASE_URI_HTTPS: %s\n", SR_BASE_URI_HTTPS.c_str());
	printf("PROVIDER_ADDRESS: %s\n", PROVIDER_ADDRESS.c_str());
	printf("PROVIDER_ADDRESS6: %s\n", PROVIDER_ADDRESS6.c_str());
	printf("CUSTOM_URL: %s\n", CUSTOM_URL.c_str());
	printf("PROVIDER_SYSTEM_NAME: %s\n", PROVIDER_SYSTEM_NAME.c_str());
	printf("SERVICE_DEFINITION: %s\n", SERVICE_DEFINITION.c_str());
	printf("INTERFACE: %s\n", INTERFACE.c_str());
	printf("PRIVATE_KEY_PATH: %s\n", PRIVATE_KEY_PATH.c_str());
	printf("PUBLIC_KEY_PATH: %s\n", PUBLIC_KEY_PATH.c_str());
	printf("PROVIDER_PORT: %i\n", PROVIDER_PORT);
	printf("SECURE_ARROWHEAD_INTERFACE: %s\n", std::to_string(SECURE_ARROWHEAD_INTERFACE).c_str());
	printf("SECURE_PROVIDER_INTERFACE: %s\n", std::to_string(SECURE_PROVIDER_INTERFACE).c_str());
}

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
	nh.param<std::string>("SR_BASE_URI", SR_BASE_URI, "http://arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("SR_BASE_URI_HTTPS", SR_BASE_URI_HTTPS, "https://arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("PROVIDER_ADDRESS", PROVIDER_ADDRESS, "10.0.0.40");
	nh.param<std::string>("PROVIDER_ADDRESS6", PROVIDER_ADDRESS6, "[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("CUSTOM_URL", CUSTOM_URL, "this_is_the_custom_url");
	nh.param<std::string>("PROVIDER_SYSTEM_NAME", PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", SERVICE_DEFINITION, "IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", PRIVATE_KEY_PATH, "keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", PUBLIC_KEY_PATH, "keys/tempsensor.testcloud1.publickey.pem");
	nh.param<int>("PROVIDER_PORT", PROVIDER_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", SECURE_PROVIDER_INTERFACE, false);
	
	bool debug = true;
	if(debug)
		debug_print();
	
	
	printf("\n=============================\nProvider Example - v%s\n=============================\n", version.c_str());
	oSensorHandler.initSensorHandler();
	convert.set(404, 1);
//SenML format
//todo:
//generate own measured value into "measuredValue"
//"value" should be periodically updated
//"sLinuxEpoch" should be periodically updated


	 while (ros::ok()) {
    	oSensorHandler.processProvider(convert.getJsonMsgs());
		ros::spin();
	}

	return 0;
}

