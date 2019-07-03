
#include "ApplicationServiceInterface.hpp"

ApplicationServiceInterface::ApplicationServiceInterface( )
{

}


ApplicationServiceInterface::~ApplicationServiceInterface()
{
	deinit();
}

// HTTP_Handler overload
int ApplicationServiceInterface::httpGETCallback(const char *Id, string *pData_str)
{
	return Callback_Serve_HTTP_GET(Id, pData_str);
}

int ApplicationServiceInterface::Callback_Serve_HTTP_GET(const char *Id, string *pData_str)
{
	*pData_str = "5678";
	return 1;
}

// HTTPs_Handler overload
int ApplicationServiceInterface::httpsGETCallback(const char *Id, string *pData_str, string _sToken, string _sSignature, string _clientDistName)
{
	return Callback_Serve_HTTPs_GET(Id, pData_str, _sToken, _sSignature, _clientDistName);
}

int ApplicationServiceInterface::Callback_Serve_HTTPs_GET(const char *Id, string *pData_str, string _sToken, string _sSignature, string _clientDistName)
{
	*pData_str = "5678";
	return 1;
}

bool ApplicationServiceInterface::init_ApplicationServiceInterface(Arrowhead_Data_ext &config)
{


	if(config.PROVIDER_ADDRESS.size() != 0){

	    URI      = "http://"  + config.PROVIDER_ADDRESS + ":" + to_string(config.PROVIDER_PORT);
	    HTTPsURI = "https://" + config.PROVIDER_ADDRESS + ":" + to_string(config.PROVIDER_PORT+1);

	    if( MakeServer(config.PROVIDER_PORT) ) {
          fprintf(stderr, "Error: Unable to start HTTP Server (%s:%d)!\n", config.PROVIDER_ADDRESS.c_str(), config.PROVIDER_PORT);
          return false;
	    }

//	    if(MakeHttpsServer(PROVIDER_PORT+1)){
//		fprintf(stderr, "Error: Unable to start HTTPs Server (%s:%d)!\n", PROVIDER_ADDRESS.c_str(), PROVIDER_PORT+1);
//		return false;
//	    }

	    printf("\n(HTTP Server) started - %s:%d\n", config.PROVIDER_ADDRESS.c_str(), config.PROVIDER_PORT);
//	    printf("(HTTPs Server) started - %s:%d\n", PROVIDER_ADDRESS.c_str(), PROVIDER_PORT+1);
	}
	else{ // ipv6
	    printf("Warning: Could not parse IPv4 address from config, trying to use IPv6!\n");

	    URI      = "http://"  + config.PROVIDER_ADDRESS6 + ":" + to_string(config.PROVIDER_PORT);
	    HTTPsURI = "https://" + config.PROVIDER_ADDRESS6 + ":" + to_string(config.PROVIDER_PORT+1);


	    if( MakeServer(config.PROVIDER_PORT) ) {
		printf("Error: Unable to start HTTP Server (%s:%d)!\n", config.PROVIDER_ADDRESS6.c_str(), config.PROVIDER_PORT);
		return false;
	    }


	    if( MakeHttpsServer(config.PROVIDER_PORT+1) ) {
		printf("Error: Unable to start HTTPs Server (%s:%d)!\n", config.PROVIDER_ADDRESS6.c_str(), config.PROVIDER_PORT+1);
		return false;
	    }

	    printf("\n(HTTP Server) started - %s:%d\n", config.PROVIDER_ADDRESS6.c_str(), config.PROVIDER_PORT);
	    printf("\n(HTTPs Server) started - %s:%d\n", config.PROVIDER_ADDRESS6.c_str(), config.PROVIDER_PORT+1);

	}

	return true;
}

int ApplicationServiceInterface::deinit( )
{
	Unload_IniFile();

	KillServer();
	KillHttpsServer();

    return 0;
}

dictionary *ApplicationServiceInterface::Load_IniFile(char *fname)
{
	pini = iniparser_load(fname);
	if( pini )
		iniparser_dump(pini, NULL);

	return pini;
}

int ApplicationServiceInterface::Unload_IniFile()
{
	if( pini )
	{
		iniparser_freedict(pini);
		pini = NULL;
		return 0;
	}
	return 1;
}

inline const char *GetHttpPayload(Arrowhead_Data_ext &config)
{

//Expected content, example:
//{
//	"providedService": {
//		"serviceDefinition" : "IndoorTemperature",
//		"interfaces" : ["json"],
//		"serviceMetadata" : {
//			"unit" : "celsius",
//			"security" : "token"
//		}
//	},
//	"provider":{
//		"systemName" : "SecureTemperatureSensor",
//		"address" : "10.0.0.2",
//		"port" : 8454,
//		"authenticationInfo" : "dfasfdsafdsa"
//	},
//	"serviceURI": "moteID/sensorID/interface-type",
//
//	"version" : 1
//	"udp": false,
//	"ttl": 255
//}

    json_object *jobj            = json_object_new_object();
    json_object *providedService = json_object_new_object();
    json_object *provider        = json_object_new_object();
    json_object *jstring;
    json_object *jint;

/*
*   ProvidedService section
*/

    jstring = json_object_new_string(config.SERVICE_DEFINITION.c_str());
    json_object_object_add(providedService, "serviceDefinition", jstring);

    json_object *jarray = json_object_new_array();
    jstring = json_object_new_string(config.INTERFACE.c_str());
    json_object_array_add(jarray, jstring);
    json_object_object_add(providedService, "interfaces", jarray);

    json_object *serviceMetadata = json_object_new_object();
    jstring = json_object_new_string(config.UNIT.c_str());
    json_object_object_add(serviceMetadata, "unit", jstring);

    if(config.AUTHENTICATION_INFO.size() != 0){
	jstring = json_object_new_string(config.SECURITY.c_str());
	json_object_object_add(serviceMetadata, "security", jstring);
    }

    json_object_object_add(providedService, "serviceMetadata", serviceMetadata);

/*
*   Provider section
*/
    jstring = json_object_new_string(config.PROVIDER_SYSTEM_NAME.c_str());
    json_object_object_add(provider, "systemName", jstring);

    jstring = json_object_new_string( config.PROVIDER_ADDRESS.size() != 0 ? config.PROVIDER_ADDRESS.c_str() : config.PROVIDER_ADDRESS6.c_str());
    json_object_object_add(provider, "address", jstring);

    if(config.AUTHENTICATION_INFO.size() != 0){

	jstring = json_object_new_string(config.AUTHENTICATION_INFO.c_str());
	json_object_object_add(provider, "authenticationInfo", jstring);

	jint = json_object_new_int(config.PROVIDER_PORT+1);
	json_object_object_add(provider, "port", jint);
    }
    else{
	jint = json_object_new_int(config.PROVIDER_PORT);
	json_object_object_add(provider, "port", jint);
    }

/*
*   Concatenation
*/

    json_object_object_add(jobj, "providedService", providedService);
    json_object_object_add(jobj, "provider", provider);

    jstring = json_object_new_string(config.CUSTOM_URL.c_str());
    json_object_object_add(jobj, "serviceURI", jstring);

    jint = json_object_new_int(1);
    json_object_object_add(jobj, "version", jint);

/*
*   Return
*/
    return json_object_to_json_string(jobj);

}

int ApplicationServiceInterface::registerToServiceRegistry(Arrowhead_Data_ext &config)
{
	if(config.SECURE_ARROWHEAD_INTERFACE)
          return SendHttpsRequest(GetHttpPayload(config), config.SR_BASE_URI_HTTPS + "register", "POST");
     else
          return SendRequest(GetHttpPayload(config), config.SR_BASE_URI + "register", "POST");
}

int ApplicationServiceInterface::unregisterFromServiceRegistry(	Arrowhead_Data_ext &config)
{
     if(config.SECURE_ARROWHEAD_INTERFACE)
          return SendHttpsRequest(GetHttpPayload(config), config.SR_BASE_URI_HTTPS + "remove", "PUT");
     else
          return SendRequest(GetHttpPayload(config), config.SR_BASE_URI + "remove", "PUT");
}
