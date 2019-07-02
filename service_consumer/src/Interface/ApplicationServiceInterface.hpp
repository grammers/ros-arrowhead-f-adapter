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
#include "../global_const.hpp"

#ifdef __linux__
     #include "ini/iniparser.h"
#elif _WIN32
     extern "C" {
     #include "ini/iniparser.h"
     }
#endif


typedef struct _Arrowhead_Data_ext
{
	std::string PUBLIC_KEY_PATH;
	std::string PRIVATE_KEY_PATH;
	std::string SERVICE_DEFINITION;
	std::string INTERFACE;
	std::string	PROVIDER_SYSTEM_NAME;
	std::string	CUSTOM_URL;
	map<std::string, std::string> SERVICE_META;
	std::string AUTHENTICATION_INFO;
	std::string SR_BASE_URI;
	std::string SR_BASE_URI_HTTPS;
	std::string PROVIDER_ADDRESS;
	std::string PROVIDER_ADDRESS6;
	int PROVIDER_PORT;
	bool SECURE_ARROWHEAD_INTERFACE;
	bool SECURE_PROVIDER_INTERFACE;
} Arrowhead_Data_ext;

class ApplicationServiceInterface :
    Http_Handler,
    Https_Handler
{
private:
	dictionary  *pini = NULL;

	std::string URI;
	std::string HTTPsURI;

	dictionary *Load_IniFile(char *fname);
	int Unload_IniFile();

public:
	ApplicationServiceInterface();
	~ApplicationServiceInterface();

	bool init_ApplicationServiceInterface(Arrowhead_Data_ext &config);
	int deinit( );
	int registerToServiceRegistry(Arrowhead_Data_ext &stAH_data);
	int unregisterFromServiceRegistry(Arrowhead_Data_ext &stAH_data);


	int httpGETCallback(const char *Id, std::string *pData_str);
	int httpsGETCallback(const char *Id, std::string *pData_str, std::string param_token, std::string param_signature, std::string clientDistName);

	virtual int Callback_Serve_HTTP_GET(const char *Id, std::string *pData_str);
	virtual int Callback_Serve_HTTPs_GET(const char *Id, std::string *pData_str, std::string param_token, std::string param_signature, std::string clientDistName);
};
