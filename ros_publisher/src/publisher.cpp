#include "ros/ros.h"
#include "sensor_msgs/Temperature.h"
#include "messages.hpp"

#pragma warning(disable:4996)
#include "Consumer/SensorHandler.h"

//AH setup
SensorHandler oSensorHandler;
// class that convert json msgs form arrowhead to ros msgs
Converter convert;

int main(int argc, char **argv)
{
	//ros setup
	ros::init(argc, argv, "client_example");
	ros::NodeHandle n;

	// declare a publisher used for debugging and observation
	// set megs type to std_msgs::Float32, publish on topic "client_demo" and use buffer of size 10
	ros::Publisher rescived_pub = n.advertise<sensor_msgs::Temperature>("temperature_example", 10);
	
	//prams
	ros::NodeHandle nh("~");
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", oSensorHandler.config.SECURE_PROVIDER_INTERFACE , false);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", oSensorHandler.config.SECURE_ARROWHEAD_INTERFACE , false);
	nh.param<std::string>("ACCESS_URI", oSensorHandler.config.ACCESS_URI ,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<std::string>("ACCESS_URI_HTTPS", oSensorHandler.config.ACCESS_URI_HTTPS ,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	nh.param<int>("THIS_PORT", oSensorHandler.config.THIS_PORT, 8454);
	nh.param<std::string>("THIS_ADDRESS", oSensorHandler.config.THIS_ADDRESS, "10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", oSensorHandler.config.THIS_ADDRESS6, "[fe80::1a57:58d9:c43:6319]");
	nh.param<std::string>("THIS_SYSTEM_NAME", oSensorHandler.config.THIS_SYSTEM_NAME, "client1");
	nh.param<std::string>("AUTHENTICATION_INFO", oSensorHandler.config.AUTHENTICATION_INFO, "null");
	nh.param<std::string>("SERVICE_DEFINITION", oSensorHandler.config.SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("INTERFACE", oSensorHandler.config.INTERFACE, "REST-JSON-SENML");
	nh.param<std::string>("SECURITY", oSensorHandler.config.SECURITY, "toke");
	nh.param<bool>("OVERRIDE_STORE", oSensorHandler.config.OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", oSensorHandler.config.MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", oSensorHandler.config.METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", oSensorHandler.config.PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", oSensorHandler.config.ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", oSensorHandler.config.EXTERNAL_SERVICE_REQUEST, false);
	nh.param<std::string>("TARGET_SYSTEM_NAME", oSensorHandler.config.TARGET_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("TARGET_ADDRESS", oSensorHandler.config.TARGET_ADDRESS, "10.10.10.122");
	nh.param<int>("TARGET_PORT", oSensorHandler.config.TARGET_PORT, 8452);
	nh.param<std::string>("UNIT", oSensorHandler.config.UNIT, "NOT SPIFFED");	

	// Print the configure parameters
	oSensorHandler.config.print();

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
	
	// set a frequency for request in Hz
	// warning: it in not exact no grantees on actual frequency
	ros::Rate loop_rate(0.3);

	int i =0;
	// loops call to provider
	while(ros::ok())
	{
    	// update the message to a new one
		convert.set(i++, std::time(0));
		
		// arrowhead ask provider for data
    	oSensorHandler.processConsumer(convert.getJsonMsgs());


		// Publish data for debug
		rescived_pub.publish(convert.temperature);
		
		// sleep until it is time for next request
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 1;
}
