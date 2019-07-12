#pragma once

#include "HttpHandler.h"
#include "HttpsHandler.h"
#include "ArrowheadDataExt.h"
#include <json-c/json.h>

namespace arrowhead{

// TODO split http and https parts
class OrchestratorInterface : HttpHandler, HttpsHandler {

private:

	std::string URI;
protected:
	std::string target_uri;

	std::string sConsumerID;

	OrchestratorInterface();
	~OrchestratorInterface();
	
	// GET or POST
	// to send/get msgs
	void sendRequestToProvider(std::string data, 
					std::string provider_uri, std::string method);
	
	// initialisation get a orchestration connection
	bool getOrchetrationRequestForm(json_object *&request_form,
									ArrowheadDataExt &config);
		
	int sendOrchestrationRequest(std::string rResult, ArrowheadDataExt *config);
	size_t CallbackOrchestrationResponse(char *ptr, size_t size);

	// callback path for GET
	// @override
	size_t callbackGETHttp(char *ptr, size_t size);
	virtual size_t callbackRequest(const char *ptr, size_t size);

	
	size_t httpResponseCallback(char *ptr, size_t size);
	size_t httpsResponseCallback(char *ptr, size_t size);
};
}
