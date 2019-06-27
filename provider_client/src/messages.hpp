#pragma once
#include "json-c/json.h"
#include <string>
#include "std_msgs/Float32.h"


class Converter {

public:
	Converter();
	~Converter();

	static std_msgs::Float32 temperature;
	//static float temperature;
	void parce(char* ptr);
};
