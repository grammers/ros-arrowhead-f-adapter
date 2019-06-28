#pragma once

#include "../Interface/OrchestratorInterface.hpp"
#include "ConsumedService.h"
#include "../const_global.hpp"
#include "../messages.hpp"

class SensorHandler : OrchestratorInterface
{

public:
	SensorHandler();
	~SensorHandler();

	void processConsumer(std::string consumerID);

	//Overload - OrchestratorInterface callback
	size_t Callback_OrchestrationResponse(char *ptr, size_t size);

     void sendRequestToProvider(std::string _sProviderURI);
     void sendHttpsRequestToProvider(std::string _sProviderURI);
     size_t providerHttpResponseCallback(char *ptr, size_t size);

	/*ConsumedServices*/
	ConsumedService oConsumedService;

private:
	// convert and stor respons in these class
	Converter oConverter;
};
