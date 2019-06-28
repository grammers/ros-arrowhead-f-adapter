#pragma once

#include "../const_global.hpp"

class ConsumedService {

public:
	ConsumedService();
	~ConsumedService();

	// Construct the request msgs that are set so be send to arrowhead for system set up
	bool getRequestForm(std::string consumerID, std::string &rRequestForm);
};
