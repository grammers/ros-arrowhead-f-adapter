#include "ros/ros.h"
#include "provider_client/clientExample.h"
#include "std_msgs/Float32.h"
#include <sstream>


#pragma warning(disable:4996)
#include "Consumer/data.cpp"
#include "Consumer/SensorHandler.h"
#include <sstream>
#include <string>
#include <stdio.h>
#include <thread>
#include <list>

#include <unistd.h>

// whot for???
//const std::stringversion = "4.0";


// TODO change to ros seting interface
bool bSecureProviderInterface = false;
bool bSecureArrowheadInterface = false;

inline void parseArguments(int argc, char* argv[]){
     for(int i=1; i<argc; ++i){
          if(strstr("--secureArrowheadInterface", argv[i]))
               bSecureArrowheadInterface = true;
          else if(strstr("--secureProviderInterface", argv[i]))
               bSecureProviderInterface = true;
     }
}


int main(int argc, char **argv)
{
	//ros setup
	ros::init(argc, argv, "client_example");
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<provider_client::clientExample>("client_example");
	ros::Publisher rescived_pub = n.advertise<std_msgs::Float32>("client_demo", 10);


	//AH setup
	parseArguments(argc, argv);
	sleep(1);
	SensorHandler oSensorHandler;
	oSensorHandler.bSecureProviderInterface = bSecureProviderInterface;

		//json - SenML format
	std::string consumerMessageExample = 
		(std::string)"{\"bn\":\"TestconsumerID\"\n}";

	ros::Rate loop_rate(0.5);

	// set upp comunication variabels
	provider_client::clientExample srv;
	std_msgs::Float32 val;

	// loops call to provider
	while(ros::ok())
	{

		// AH ask provider
    	oSensorHandler.processConsumer(
			consumerMessageExample, bSecureArrowheadInterface);


		//set upp to publich reseved data
		val.data = oSensorHandler.temp;
		// Publich data for debug
		rescived_pub.publish(val);
//		
//		cout<<oSensorHandler.temp<<endl;
		//cout<<"whot is it: "<<temperature<<endl;
		// sends a request to ros::server
		// TODO change to sent to AH provider
		// 	change test to chek if reseved form AH
/*		if (client.call(srv))
		{	
			//set upp to publich reseved data
			val.data = srv.response.temp;
			
			//print response
			//ROS_INFO("Sum: %I64d", srv.response.temp);
			
			// Publich data for debug
			rescived_pub.publish(val);
		}
		else
		{
			ROS_ERROR("Faild to call provider");
			return 1;
*///		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 1;
}
