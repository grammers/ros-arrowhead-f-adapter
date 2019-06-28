#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "const_global.hpp"
#include "messages.hpp"

#pragma warning(disable:4996)
#include "Consumer/SensorHandler.h"

// Convet boolean ("x") to string "true"/"false"
// requierd to print booleans ass string
#define b2s(x) ((x)?"true":"false") 

// arrowhead system seting
// do not temper wiht these in code (unles you know wot you are doing)
// is suposed to be defined as const, but that do not comple.
bool SECURE_PROVIDER_INTERFACE;
bool SECURE_ARROWHEAD_INTERFACE;
std::string OR_BASE_URI;
std::string OR_BASE_URI_HTTPS;
int CLIENT_PORT;
std::string CLIENT_ADDRESS;
std::string CLIENT_ADDRESS6;
std::string CONSUMER_ID;
std::string CLIENT_SYSTEM_NAME;
std::string AUTHENTICATION_INFO;
std::string SERVICE_DEFINITION;
std::string INTERFACES;
std::string SECURITY;
bool OVERRIDE_STORE;
bool MATCHMAKING;
bool METADATA_SEARCH;
bool PING_PROVIDERS;
bool ONLY_PREFERRED;
bool EXTERNAL_SERVICE_REQUEST;
std::string PROVIDER_SYSTEM_NAME;
std::string PROVIDER_ADDRESS;
int PROVIDER_PORT;

// to check sow it setings parameters has registerd
void debugPrint(){
	printf("\n");
	printf("bSecutePrivuderIntrerface: %s\n", b2s(SECURE_PROVIDER_INTERFACE));
	printf("cureArrowheadInterface: %s\n", b2s(SECURE_ARROWHEAD_INTERFACE));
	printf("or_base_uri: %s\n", OR_BASE_URI.c_str());
	printf("or_base_uri_https: %s\n", OR_BASE_URI_HTTPS.c_str());
	printf("client_port: %i\n", CLIENT_PORT);
	printf("client_address: %s\n", CLIENT_ADDRESS.c_str());
	printf("client_address6: %s\n", CLIENT_ADDRESS6.c_str());
	printf("consumer_id: %s\n", CONSUMER_ID.c_str());
	printf("client_system_name: %s\n", CLIENT_SYSTEM_NAME.c_str());
	printf("AUTHENTICATION_INFO: %s\n", AUTHENTICATION_INFO.c_str());
	printf("SERVICE_DEFINITION: %s\n", SERVICE_DEFINITION.c_str());
	printf("INTERFACES: %s\n", INTERFACES.c_str());
	printf("SECURITY: %s\n", SECURITY.c_str());
	printf("OVERRIDE_STORE: %s\n", b2s(OVERRIDE_STORE));
	printf("MATCHMAKING: %s\n", b2s(MATCHMAKING));
	printf("METADATA_SEARCH: %s\n", b2s(METADATA_SEARCH));
	printf("PING_PRVIDERS: %s\n", b2s(PING_PROVIDERS));
	printf("ONLY_PREFERRED: %s\n", b2s(ONLY_PREFERRED));
	printf("EXTERNAL_SERVICE_REQUEST: %s\n", b2s(EXTERNAL_SERVICE_REQUEST));
	printf("PROVIDER_SYSTEM_NAME: %s\n", PROVIDER_SYSTEM_NAME.c_str());
	printf("PROVIDER_ADDRESS: %s\n", PROVIDER_ADDRESS.c_str());
	printf("PROVIDER_PROT: %i\n", PROVIDER_PORT);
}

int main(int argc, char **argv)
{
	//ros setup
	ros::init(argc, argv, "client_example");
	ros::NodeHandle n;

	// declar a publisher used for debugin and observation
	// set megs type to std_msgs::Float32, publish on topic "client_demo" and use buffer of size 10
	ros::Publisher rescived_pub = n.advertise<std_msgs::Float32>("client_demo", 10);

	//params
	ros::NodeHandle nh("~");
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", SECURE_PROVIDER_INTERFACE , false);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", SECURE_ARROWHEAD_INTERFACE , false);
	nh.param<std::string>("OR_BASE_URI", OR_BASE_URI ,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<std::string>("OR_BASE_URI_HTTPS", OR_BASE_URI_HTTPS ,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	nh.param<int>("CLIENT_PORT", CLIENT_PORT, 8454);
	nh.param<std::string>("CLIENT_ADDRESS", CLIENT_ADDRESS, "10.0.0.40");
	nh.param<std::string>("CLIENT_ADDRESS6", CLIENT_ADDRESS6, "[fe80::1a57:58d9:c43:6319]");
	nh.param<std::string>("CONSUMER_ID",CONSUMER_ID, "TestconsumerID");
	nh.param<std::string>("CLIENT_SYSTEM_NAME", CLIENT_SYSTEM_NAME, "client1");
	nh.param<std::string>("AUTHENTICATION_INFO", AUTHENTICATION_INFO, "null");
	nh.param<std::string>("SERVICE_DEFINITION", SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("INTERFACES", INTERFACES, "REST-JSON-SENML");
	nh.param<std::string>("SECURITY", SECURITY, "toke");
	nh.param<bool>("OVERRIDE_STORE", OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", EXTERNAL_SERVICE_REQUEST, false);
	nh.param<std::string>("PROVIDER_SYSTEM_NAME", PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("PROVIDER_ADDRESS", PROVIDER_ADDRESS, "10.10.10.122");
	nh.param<int>("PROVIDER_PORT", PROVIDER_PORT, 8452);
	
	bool debug = true;
	if(debug)
		debugPrint();

	// class that conver json msgs form arrowhead to ros msgs
	Converter oConverter;

	//AH setup
	SensorHandler oSensorHandler;

		//json - SenML format
	std::string consumerMessageExample = 
		(std::string)"{\"bn\":\"TestconsumerID\"\n}";

	// set a frequens for request in Hz
	// warning it in not exact no garantes on aktual frequens
	ros::Rate loop_rate(0.3);


	// loops call to provider
	while(ros::ok())
	{

		// arrowhead ask provider for data
    	oSensorHandler.processConsumer(consumerMessageExample);


		// Publich data for debug
		rescived_pub.publish(oConverter.temperature);
		
		// sleep entil it is time for next request
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 1;
}
