#include "ros/ros.h" 
#include "sensor_msgs/Temperature.h" 
#include "../../comon/messages.hpp"

#pragma warning(disable:4996) 
#include "arrowhead/publisher.h" 
#include "arrowhead/consumer.h" 

using namespace arrowhead;

//AH setup
Consumer consumer; 
Publisher publisher;
// class that convert json msgs form arrowhead to ros msgs
Converter convert;

int main(int argc, char **argv) {
	//ros setup
	ros::init(argc, argv, "consumer_publisher"); 
	ros::NodeHandle n;

	// declare a publisher used for debugging and observation set megs type to
	// std_msgs::Float32, publish on topic "client_demo" and use buffer of size
	// 10
	ros::Publisher rescived_pub =
			n.advertise<sensor_msgs::Temperature>("temperature_consumer", 10);
	ros::Publisher tf_pub = 
			n.advertise<sensor_msgs::Temperature>("temperature_publisher", 10);
	
	//prams
	ros::NodeHandle nh("~");
	// service
	nh.param<std::string>("SERVICE_DEFINITION_CONSUMER",
					consumer.config.SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("UNIT_CONSUMER", consumer.config.UNIT, "NOT SPIFFED");
	nh.param<std::string>("SERVICE_DEFINITION_PUBLISHER",
					publisher.config.SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("UNIT_PUBLISHER", publisher.config.UNIT, 
					"NOT SPIFFED");	
	// target
	nh.param<std::string>("TARGET_SYSTEM_NAME_CONSUMER",
					consumer.config.TARGET_SYSTEM_NAME,
					"SecureTemperatureSensor");
	nh.param<std::string>("TARGET_ADDRESS_CONSUMER",
					consumer.config.TARGET_ADDRESS, "10.10.10.122");
	nh.param<int>("TARGET_PORT_CONSUMER", consumer.config.TARGET_PORT, 8452);
	nh.param<std::string>("TARGET_SYSTEM_NAME_PUBLISHER",
					publisher.config.TARGET_SYSTEM_NAME,
					"SecureTemperatureSensor");
	nh.param<std::string>("TARGET_ADDRESS_PUBLISHER",
					publisher.config.TARGET_ADDRESS, "10.10.10.122");
	nh.param<int>("TARGET_PORT_PUBLISHER", publisher.config.TARGET_PORT, 8452);
	
	//nh.param<std::string>("AUTHENTICATION_INFO",
	//oSensorHandler.config.AUTHENTICATION_INFO, "null"); this	
	nh.param<int>("THIS_PORT", consumer.config.THIS_PORT, 8454);
	nh.param<std::string>("THIS_ADDRESS", consumer.config.THIS_ADDRESS,
					"10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", consumer.config.THIS_ADDRESS6,
					"[fe80::1a57:58d9:c43:6319]");
	nh.param<std::string>("THIS_SYSTEM_NAME", consumer.config.THIS_SYSTEM_NAME,
					"client1");
	// service some
	nh.param<std::string>("INTERFACE", consumer.config.INTERFACE,
					"REST-JSON-SENML"); 
	nh.param<std::string>("SECURITY",
							consumer.config.SECURITY, "toke");
	nh.param<bool>("SECURE_PROVIDER_INTERFACE",
					consumer.config.SECURE_PROVIDER_INTERFACE , false);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE",
					consumer.config.SECURE_ARROWHEAD_INTERFACE , false);
	// core some
	nh.param<std::string>("ACCESS_URI", consumer.config.ACCESS_URI
					,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<std::string>("ACCESS_URI_HTTPS", consumer.config.ACCESS_URI_HTTPS
					,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	// orchestrator
	nh.param<bool>("OVERRIDE_STORE", consumer.config.OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", consumer.config.MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", consumer.config.METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", consumer.config.PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", consumer.config.ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST",
					consumer.config.EXTERNAL_SERVICE_REQUEST, false);

	publisher.config.AUTHENTICATION_INFO = consumer.config.AUTHENTICATION_INFO;
	publisher.config.THIS_PORT = consumer.config.THIS_PORT;
	publisher.config.THIS_ADDRESS = consumer.config.THIS_ADDRESS;
	publisher.config.THIS_ADDRESS6 = consumer.config.THIS_ADDRESS6;
	publisher.config.THIS_SYSTEM_NAME = consumer.config.THIS_SYSTEM_NAME;
	publisher.config.INTERFACE = consumer.config.INTERFACE;
	publisher.config.SECURITY = consumer.config.SECURITY;
	publisher.config.SECURE_PROVIDER_INTERFACE =
			consumer.config.SECURE_PROVIDER_INTERFACE;
	publisher.config.SECURE_ARROWHEAD_INTERFACE =
			consumer.config.SECURE_ARROWHEAD_INTERFACE;
	publisher.config.ACCESS_URI = consumer.config.ACCESS_URI;
	publisher.config.ACCESS_URI_HTTPS = consumer.config.ACCESS_URI_HTTPS;
	publisher.config.OVERRIDE_STORE = consumer.config.OVERRIDE_STORE;
	publisher.config.MATCHMAKING = consumer.config.MATCHMAKING;
	publisher.config.PING_PROVIDERS = consumer.config.PING_PROVIDERS;
	publisher.config.ONLY_PREFERRED = consumer.config.ONLY_PREFERRED;
	publisher.config.EXTERNAL_SERVICE_REQUEST =
			consumer.config.EXTERNAL_SERVICE_REQUEST;


	// Print the configure parameters
	printf("consumer config\n"); 
	consumer.config.print(); 
	printf("publisher config\n"); 
	publisher.config.print();

	// Set up the message 
	// param @ sensor_id identification name 
	// (suggest, use same as service id in arrowhead database) 
	// @ unit the unit that the data are sent in 
	// @ baseName must be some as above
	convert.init("sensor_id", publisher.config.UNIT,
					publisher.config.THIS_SYSTEM_NAME);

	ros::Rate loop_rate(0.3);
	// there is a race condition
	// these loops are to wait out other nods to initialise
	// and thus eliminate the effect of the race condition
	while(!publisher.init()){
		fprintf(stderr, "retry connecting in a moment\n");
		loop_rate.sleep();
	}
	
	while(!consumer.init(Converter::parce)){
		fprintf(stderr, "retry connecting in a moment\n");
		loop_rate.sleep();
	}


	// to reserve msgs mast it be define first should not be don her. 
	// But good
	// for testing TODO set it to whit for first update form sensor
	convert.set(4, 1);
	
	// set a frequency for request in Hz 
	// warning: it in not exact no grantees, on actual frequency

	// loops call to provider
	while(ros::ok()) {
		// sleep until it is time for next request
		ros::spinOnce(); 
		loop_rate.sleep();

		// get temperature reading from provider
		consumer.request();

		// arrowhead ask provider for data
		convert.set(((Converter::temperature.temperature * 1.8)+32),
						Converter::temperature.header.stamp.sec); 
		publisher.publish(convert.getJsonMsgs());

		// Publish data for debug
		rescived_pub.publish(convert.temperature);
		
		sensor_msgs::Temperature farenhite;
		farenhite.temperature = ((Converter::temperature.temperature * 1.8) + 32);
		tf_pub.publish(farenhite);

	} 
	return 1;
}
