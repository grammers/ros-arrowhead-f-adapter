#pragma once
#include "sensor_msgs/Temperature.h"
//#include <ros/ros.h>
#include "json-c/json.h"

class Converter {

public:
	Converter();
	~Converter();

	// the message reserved in ROS msgs format
	static sensor_msgs::Temperature temperature;
	
	// a set up to set private variables for msgs building
	// param
	// @ sensor_id Identification name the sensor
	// @ unit the unit of the requested data
	// @ baseName identification name for the procomer
	void init(std::string sensor_id, std::string unit, std::string baseName);

	// parse returning msgs ("pte") from provider to ROS msgs
	static void parce(const char* url, const char* ptr);
	
	// Create a new msgs
	void set(double temperature, int t_stamp);

	void updateMsgs();

	// returns the msgs
	json_object* getJsonMsgs();

private:
	json_object *obj;
	std::string sensor_id;
	std::string data_unit;
	std::string identety;
};
