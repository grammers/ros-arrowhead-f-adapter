#pragma once


#include "HttpHandler.h"
#include "HttpsHandler.h"
#include "ArrowheadDataExt.h"


namespace arrowhead{

// TODO split up into http and https handling classes
class ApplicationServiceInterface :
    HttpHandler,
    HttpsHandler
{
private:

	std::string URI;
	std::string HTTPs_URI;
	
	bool createServer(std::string ip, int port);

protected:
	bool sensorIsRegistered;

//public:
	ApplicationServiceInterface();
	~ApplicationServiceInterface();

	// Init system
	// create server to enable http request
	// it cascade to createServer and MakeServer
	bool initApplicationServiceInterface(ArrowheadDataExt &config);
	bool registerSensor(ArrowheadDataExt &config, std::string base_name);
	int deinit( );
	
	// register to service registry
	int registerToServiceRegistry(ArrowheadDataExt &stAH_data);
	int unregisterFromServiceRegistry(ArrowheadDataExt &stAH_data);

	// callbacks form http requests
	// it require to override implementation
	// GET request received
	// @override
	int httpGETCallback(const char *Id, std::string *data_str);
	virtual int callbackServerHttpGET(const char *Id, 
					std::string *data_str);
	// https
	// @override
	int httpsGETCallback(const char *Id, std::string *data_str, 
					std::string param_token, 
					std::string param_signature, 
					std::string client_dist_name);
	virtual int callbackServerHttpsGET(const char *Id, 
					std::string *data_str, 
					std::string param_token, 
					std::string param_signature, 
					std::string client_dist_name);

	// POST request received
	// @override
	int httpPOSTCallback(const char *url, const char *payload);
	virtual int callbackServerHttpPOST(const char *url, 
					const char *payload);
};
}
