#pragma once
#include "std_msgs/Float32.h"
#include <ros/ros.h>
#include "Interface/include/curl/curl.h"
#include "microhttpd.h"
#include "json-c/json.h"

class Converter {

public:
	Converter();
	~Converter();

	void init(std::string sensor_id, std::string unit, std::string baseName);
	// the message reserved in ROS msgs format
	static std_msgs::Float32 temperature;
	// parse returning msgs ("pte") from provider to ROS msgs
	void parce(char* ptr);
	void set(double temperature, int t_stamp);
	json_object* getJsonMsgs();
private:
	json_object *obj;
	std::string sensor_id;
	std::string data_unit;
	std::string identety;
};
