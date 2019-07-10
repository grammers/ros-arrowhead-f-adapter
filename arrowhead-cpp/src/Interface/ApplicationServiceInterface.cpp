
#include "ApplicationServiceInterface.hpp"
namespace arrowhead{

ApplicationServiceInterface::ApplicationServiceInterface( )
{

}


ApplicationServiceInterface::~ApplicationServiceInterface()
{
	deinit();
}

//////////////////////////
// http/https callbacks //
//////////////////////////

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

// HTTP_Hander overload POST
int ApplicationServiceInterface::httpPOSTCallback(const char *url, const char *payload){
	printf("POST callback to application: %s\n", payload);
	return Callback_Serve_HTTP_POST(url, payload);
}

int ApplicationServiceInterface::Callback_Serve_HTTP_POST(const char *url, const char *payload){
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
////////////////////
// callbacks ends //
////////////////////



bool ApplicationServiceInterface::init_ApplicationServiceInterface(Arrowhead_Data_ext &config)
{
	if(config.THIS_ADDRESS.size() != 0){
		return createServer(config.THIS_ADDRESS, config.THIS_PORT);
	}
	else{ // ipv6
	    printf("Warning: Could not parse IPv4 address from config, trying to use IPv6!\n");

		return createServer(config.THIS_ADDRESS6, config.THIS_PORT);
	}
}

bool ApplicationServiceInterface::createServer(std::string ip, int port){
	// https is not in use
	URI      = "http://"  + ip + ":" + to_string(port);
	//HTTPsURI = "https://" + ip + ":" + to_string(port+1);


	if( MakeServer(port) ) {
		printf("Error: Unable to start HTTP Server (%s:%d)!\n", ip.c_str(), port);
		return false;
    }


    //if( MakeHttpsServer(port+1) ) {
	//	printf("Error: Unable to start HTTPs Server (%s:%d)!\n", ip.c_str(), port+1);
	//	return false;
    //}

	printf("\n(HTTP Server) started - %s:%d\n", ip.c_str(), port);
	//printf("\n(HTTPs Server) started - %s:%d\n", ip.c_str(), port+1);
	return true;
}

int ApplicationServiceInterface::deinit( )
{
	KillServer();
	KillHttpsServer();

    return 0;
}

// creates a json of some of the params in config
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
    jstring = json_object_new_string(config.THIS_SYSTEM_NAME.c_str());
    json_object_object_add(provider, "systemName", jstring);

    jstring = json_object_new_string( config.THIS_ADDRESS.size() != 0 ? config.THIS_ADDRESS.c_str() : config.THIS_ADDRESS6.c_str());
    json_object_object_add(provider, "address", jstring);

    if(config.AUTHENTICATION_INFO.size() != 0){

	jstring = json_object_new_string(config.AUTHENTICATION_INFO.c_str());
	json_object_object_add(provider, "authenticationInfo", jstring);

	jint = json_object_new_int(config.THIS_PORT+1);
	json_object_object_add(provider, "port", jint);
    }
    else{
	jint = json_object_new_int(config.THIS_PORT);
	json_object_object_add(provider, "port", jint);
    }

/*
*   Concatenation
*/

    json_object_object_add(jobj, "providedService", providedService);
    json_object_object_add(jobj, "provider", provider);

    jstring = json_object_new_string(config.SERVICE_URI.c_str());
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
          return SendHttpsRequest(GetHttpPayload(config), config.ACCESS_URI_HTTPS + "register", "POST");
     else
          return SendRequest(GetHttpPayload(config), config.ACCESS_URI + "register", "POST");
}

int ApplicationServiceInterface::unregisterFromServiceRegistry(	Arrowhead_Data_ext &config)
{
     if(config.SECURE_ARROWHEAD_INTERFACE)
          return SendHttpsRequest(GetHttpPayload(config), config.ACCESS_URI_HTTPS + "remove", "PUT");
     else
          return SendRequest(GetHttpPayload(config), config.ACCESS_URI + "remove", "PUT");
}
}
