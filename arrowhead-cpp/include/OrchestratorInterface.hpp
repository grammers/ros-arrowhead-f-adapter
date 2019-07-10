#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <list>

#include "Http_Handler.hpp"
#include "Https_Handler.hpp"
#include "arrowhead_data_ext.hpp"

#include "iniparser.h"

namespace arrowhead{
using namespace std;
class OrchestratorInterface : Http_Handler, Https_Handler {

private:

	string URI;

public:
	string sConsumerID;

	OrchestratorInterface();
	~OrchestratorInterface();
	
	void sendRequestToProvider(std::string data, std::string provider_uri, std::string method);
	size_t callback_GET_http(char *ptr, size_t size);
	virtual size_t callback_GET(const char *ptr, size_t size);


	bool init_OrchestratorInterface(Arrowhead_Data_ext *config);
	int deinit();

	int sendOrchestrationRequest(string rResult, Arrowhead_Data_ext *config);

	size_t httpResponseCallback(char *ptr, size_t size);
	size_t httpsResponseCallback(char *ptr, size_t size);

	virtual size_t Callback_OrchestrationResponse(char *ptr, size_t size);
};
}
