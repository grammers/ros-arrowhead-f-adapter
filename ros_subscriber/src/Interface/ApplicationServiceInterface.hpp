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

#ifdef __linux__
     #include "ini/iniparser.h"
#elif _WIN32
     extern "C" {
     #include "ini/iniparser.h"
     }
#endif

// struct contains all configure variables
typedef struct _Arrowhead_Data_ext
{
	std::string PUBLIC_KEY_PATH;
	std::string PRIVATE_KEY_PATH;
	std::string SERVICE_DEFINITION;
	std::string INTERFACE;
	std::string	THIS_SYSTEM_NAME;
	std::string	CUSTOM_URL;
	std::string UNIT;
	std::string SECURITY;
	std::string AUTHENTICATION_INFO;
	std::string SR_BASE_URI;
	std::string SR_BASE_URI_HTTPS;
	std::string THIS_ADDRESS;
	std::string THIS_ADDRESS6;
	int THIS_PORT;
	bool SECURE_ARROWHEAD_INTERFACE;
	bool SECURE_THIS_INTERFACE;

	void print(){
		printf("The set params");
		printf("PUBLIC_KEY_PATH: %s\n", PUBLIC_KEY_PATH.c_str());
		printf("PRIVATE_KEY_PATH: %s\n", PRIVATE_KEY_PATH.c_str());
		printf("SERVICE_DEFINITION: %s\n", SERVICE_DEFINITION.c_str());
		printf("INTERFACE: %s\n", INTERFACE.c_str());
		printf("THIS_SYSTEM_NAME: %s\n", THIS_SYSTEM_NAME.c_str());
		printf("CUSTOM_URL: %s\n", CUSTOM_URL.c_str());
		printf("UNIT : %s\n", UNIT.c_str());
		printf("SECURITY: %s\n", SECURITY.c_str());
		printf("AUTHENTICATION_INFO: %s\n", AUTHENTICATION_INFO.c_str());
		printf("SR_BASE_URI: %s\n", SR_BASE_URI.c_str());
		printf("SR_BASE_URI_HTTPS: %s\n", SR_BASE_URI_HTTPS.c_str());
		printf("THIS_ADDRESS: %s\n", THIS_ADDRESS.c_str());
		printf("THIS_ADDRESS6: %s\n", THIS_ADDRESS6.c_str());
		printf("THIS_PORT: %i\n", THIS_PORT);
		printf(":SECURE_ARROWHEAD_INTERFACE %s\n", std::to_string(SECURE_ARROWHEAD_INTERFACE).c_str());
		printf("SECURE_THIS_INTERFACE: %s\n", std::to_string(SECURE_THIS_INTERFACE).c_str());
	}
} Arrowhead_Data_ext;

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