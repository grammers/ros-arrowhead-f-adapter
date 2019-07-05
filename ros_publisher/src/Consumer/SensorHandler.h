#pragma once

#include "../Interface/OrchestratorInterface.hpp"
#include "ConsumedService.h"
#include "arrowhead_data_ext.hpp"
#include "json-c/json.h"

class SensorHandler : OrchestratorInterface
{

public:
	SensorHandler();
	~SensorHandler();
	
	Arrowhead_Data_ext config;
	json_object *msg;

	void processConsumer(json_object *pJsonSenML);

	//Overload - OrchestratorInterface callback
	size_t Callback_OrchestrationResponse(char *ptr, size_t size);

     void sendRequestToProvider(std::string _sProviderURI);
     void sendHttpsRequestToProvider(std::string _sProviderURI);
     size_t providerHttpResponseCallback(char *ptr, size_t size);

	/*ConsumedServices*/
	ConsumedService oConsumedService;

private:
};
