#include "ros/ros.h"
#include "provider_client/clientExample.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include "const_global.hpp"
#include "messages.hpp"

#pragma warning(disable:4996)
#include "Consumer/SensorHandler.h"
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <thread>
#include <list>

#include <unistd.h>

using namespace std;

// whot for???
//const std::stringversion = "4.0";


// securety seting 
bool bSecureProviderInterface;
bool bSecureArrowheadInterface;

// arrowhead system seting
// do not temper wiht these in code (unles you know wot you are doing)
// is suposed to be defined as const, but that do not comple.
string OR_BASE_URI;
string OR_BASE_URI_HTTPS;
int CLIENT_PORT;
string CLIENT_ADDRESS;
string CLIENT_ADDRESS6;
string CONSUMER_ID;
string CLIENT_SYSTEM_NAME;
string AUTHENTICATION_INFO;
string SERVICE_DEFINITION;
string INTERFACES;
string SECURITY;
bool OVERRIDE_STORE;
bool MATCHMAKING;
bool METADATA_SEARCH;
bool PING_PROVIDERS;
bool ONLY_PREFERRED;
bool EXTERNAL_SERVICE_REQUEST;
string PROVIDER_SYSTEM_NAME;
string PROVIDER_ADDRESS;
int PROVIDER_PORT;


void debugPrint(){
cout<<endl;
cout<<"bSecutePrivuderIntrerface: "<<bSecureProviderInterface<<endl;
cout<<"bSecureArrowheadInterface: "<<bSecureArrowheadInterface<<endl;
cout<<"or_base_uri: "<<OR_BASE_URI<<endl;
cout<<"or_base_uri_https: "<<OR_BASE_URI_HTTPS<<endl;
cout<<"client_port: "<<CLIENT_PORT<<endl;
cout<<"client_address: "<<CLIENT_ADDRESS<<endl;
cout<<"client_address6: "<<CLIENT_ADDRESS6<<endl;
cout<<"consumer_id: "<<CONSUMER_ID<<endl;
cout<<"client_system_name: "<<CLIENT_SYSTEM_NAME<<endl;
cout<<"AUTHENTICATION_INFO: "<<AUTHENTICATION_INFO<<endl;
cout<<"SERVICE_DEFINITION: "<<SERVICE_DEFINITION<<endl;
cout<<"INTERFACES: "<<INTERFACES<<endl;
cout<<"SECURITY: "<<SECURITY<<endl;
cout<<"OVERRIDE_STORE: "<<OVERRIDE_STORE<<endl;
cout<<"MATCHMAKING: "<<MATCHMAKING<<endl;
cout<<"METADATA_SEARCH: "<<METADATA_SEARCH<<endl;
cout<<"PING_PRVIDERS: "<<PING_PROVIDERS<<endl;
cout<<"ONLY_PREFERRED: "<<ONLY_PREFERRED<<endl;
cout<<"EXTERNAL_SERVICE_REQUEST: "<<EXTERNAL_SERVICE_REQUEST<<endl;
cout<<"PROVIDER_SYSTEM_NAME: "<<PROVIDER_SYSTEM_NAME<<endl;
cout<<"PROVIDER_ADDRESS: "<<PROVIDER_ADDRESS<<endl;
cout<<"PROVIDER_PROT: "<<PROVIDER_PORT<<endl;
}

int main(int argc, char **argv)
{
	bool debug = true;
	//ros setup
	ros::init(argc, argv, "client_example");
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<provider_client::clientExample>("client_example");
	ros::Publisher rescived_pub = n.advertise<std_msgs::Float32>("client_demo", 10);

	//params
	ros::NodeHandle nh("~");
	nh.param<bool>("bSecureProviderInterface", bSecureProviderInterface , false);
	nh.param<bool>("bSecureArrowheadInterface", bSecureArrowheadInterface , false);
	nh.param<string>("OR_BASE_URI", OR_BASE_URI ,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<string>("OR_BASE_URI_HTTPS", OR_BASE_URI_HTTPS ,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	nh.param<int>("CLIENT_PORT", CLIENT_PORT, 8454);
	nh.param<string>("CLIENT_ADDRESS", CLIENT_ADDRESS, "10.0.0.40");
	nh.param<string>("CLIENT_ADDRESS6", CLIENT_ADDRESS6, "[fe80::1a57:58d9:c43:6319]");
	nh.param<string>("CONSUMER_ID",CONSUMER_ID, "TestconsumerID");
	nh.param<string>("CLIENT_SYSTEM_NAME", CLIENT_SYSTEM_NAME, "client1");
	nh.param<string>("AUTHENTICATION_INFO", AUTHENTICATION_INFO, "null");
	nh.param<string>("SERVICE_DEFINITION", SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<string>("INTERFACES", INTERFACES, "REST-JSON-SENML");
	nh.param<string>("SECURITY", SECURITY, "toke");
	nh.param<bool>("OVERRIDE_STORE", OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", EXTERNAL_SERVICE_REQUEST, false);
	nh.param<string>("PROVIDER_SYSTEM_NAME", PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<string>("PROVIDER_ADDRESS", PROVIDER_ADDRESS, "10.10.10.122");
	nh.param<int>("PROVIDER_PORT", PROVIDER_PORT, 8452);
	
	if(debug)
		debugPrint();


	Converter oConverter;
	//AH setup
	SensorHandler oSensorHandler;
	//oSensorHandler.bSecureProviderInterface = bSecureProviderInterface;

		//json - SenML format
	std::string consumerMessageExample = 
		(std::string)"{\"bn\":\"TestconsumerID\"\n}";

	ros::Rate loop_rate(0.3);

	// set upp comunication variabels
	provider_client::clientExample srv;

	// loops call to provider
	while(ros::ok())
	{

		// AH ask provider
    	oSensorHandler.processConsumer(
			consumerMessageExample, bSecureArrowheadInterface);


		// Publich data for debug
		//std_msgs::Float32 temp;
		//temp.data = Converter::temperature;
		//temp.data = 1;
		rescived_pub.publish(oConverter.temperature);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 1;
}
