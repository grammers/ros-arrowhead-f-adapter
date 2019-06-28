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

	// the message reseved reseved in ROS msgs format
	static std_msgs::Float32 temperature;

	// parce retunring msgs ("pte") from provider to ROS msgs
	void parce(char* ptr);
};
