#pragma once

#include <map>
#include <string>
#include <json-c/json.h>
#include "../const_global.hpp"

class ConsumedService {

private:
	
	
//	std::string filePath = std::string("/home/grammers/catkin_ws/src/ros-arrowhead-f-adapter/provider_client/consumedServices.json");
//	std::string filePath = std::string("../../consumedServices.json");
	//std::string filePath = std::string(@JSON@);
	
public:
	ConsumedService();
	~ConsumedService();
	
	bool getRequestForm(std::string consumerID, std::string &rRequestForm);
};
