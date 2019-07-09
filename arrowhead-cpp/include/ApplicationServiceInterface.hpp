#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <list>

#include <map>
#include <vector>
#include <json-c/json.h>
#include "Http_Handler.hpp"
#include "Https_Handler.hpp"
#include "arrowhead_data_ext.hpp"

#ifdef __linux__
     #include "iniparser.h"
#elif _WIN32
     extern "C" {
     #include "iniparser.h"
     }
#endif


class ApplicationServiceInterface :
    Http_Handler,
    Https_Handler
{
private:

	std::string URI;
	std::string HTTPsURI;
	
	bool createServer(std::string ip, int port);

public:
	ApplicationServiceInterface();
	~ApplicationServiceInterface();

	// Init system
	// create server to enable http request
	// it cascade to createServer and MakeServer
	bool init_ApplicationServiceInterface(Arrowhead_Data_ext &config);
	int deinit( );
	
	// register to service registry
	int registerToServiceRegistry(Arrowhead_Data_ext &stAH_data);
	int unregisterFromServiceRegistry(Arrowhead_Data_ext &stAH_data);

	// callbacks form http requests
	// it redirect to override implementation
	int httpGETCallback(const char *Id, std::string *pData_str);
	int httpsGETCallback(const char *Id, std::string *pData_str, std::string param_token, std::string param_signature, std::string clientDistName);
	int httpPOSTCallback(const char *url, const char *payload);
	virtual int Callback_Serve_HTTP_POST(const char *url, const char *payload);
	virtual int Callback_Serve_HTTP_GET(const char *Id, std::string *pData_str);
	virtual int Callback_Serve_HTTPs_GET(const char *Id, std::string *pData_str, std::string param_token, std::string param_signature, std::string clientDistName);
};
