#pragma once

using namespace std;

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

using namespace std;

typedef struct _Arrowhead_Data_ext
{
	string 				sServiceDefinition;
	string 				sserviceInterface;
	string				sSystemName;
	string				sServiceURI;
	map<string, string>		vService_Meta;
	string				sAuthenticationInfo;
} Arrowhead_Data_ext;

class ApplicationServiceInterface :
    Http_Handler,
    Https_Handler
{
private:
	dictionary  *pini = NULL;

//	string SR_BASE_URI;
//	string SR_BASE_URI_HTTPS;
//	string ADDRESS;
//	string ADDRESS6;
//	unsigned short PORT;
	string URI;
	string HTTPsURI;

	dictionary *Load_IniFile(char *fname);
	int Unload_IniFile();

public:
	ApplicationServiceInterface();
	~ApplicationServiceInterface();

	bool init_ApplicationServiceInterface();
	int deinit( );
	int registerToServiceRegistry(Arrowhead_Data_ext &stAH_data);
	int unregisterFromServiceRegistry(Arrowhead_Data_ext &stAH_data);

	int httpGETCallback(const char *Id, string *pData_str);
	int httpsGETCallback(const char *Id, string *pData_str, string param_token, string param_signature, string clientDistName);

	virtual int Callback_Serve_HTTP_GET(const char *Id, string *pData_str);
	virtual int Callback_Serve_HTTPs_GET(const char *Id, string *pData_str, string param_token, string param_signature, string clientDistName);
};
