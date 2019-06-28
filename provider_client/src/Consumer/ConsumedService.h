#pragma once

#include "../const_global.hpp"

class ConsumedService {

public:
	ConsumedService();
	~ConsumedService();

	// bild and construkt the request msgs to sent to arrow head to set up system
	bool getRequestForm(std::string consumerID, std::string &rRequestForm);
};
