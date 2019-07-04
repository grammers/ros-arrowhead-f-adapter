#pragma once
#include "sensor_msgs/Temperature.h"
#include <ros/ros.h>
#include "Interface/include/curl/curl.h"
#include "microhttpd.h"
#include "json-c/json.h"

class Converter {

public:
	Converter();
	~Converter();

	// the message reserved in ROS msgs format
	static sensor_msgs::Temperature temperature;

	// parse returning msgs ("pte") from provider to ROS msgs
	void parce(char* ptr);
};
