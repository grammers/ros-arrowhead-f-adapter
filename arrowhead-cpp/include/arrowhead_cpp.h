#pragma once

#include "ApplicationServiceInterface.hpp"
#include "arrowhead_data_ext.hpp"
#include "OrchestratorInterface.hpp"

namespace arrowhead{

// for callbacks
//using f_void_f = void(*)(const char*, const char*);

class Arrowhead : ApplicationServiceInterface, OrchestratorInterface
{
public:
	Arrowhead();
	~Arrowhead();

	// configuration struct containing all values for setting up the procumer
	Arrowhead_Data_ext config;
	
	// initialisation cat be dun in constructor because the values are sett later.
	void init_provider(std::string base_name);
	void init_consumer(f_void_f callback);
	void init_publisher();
	void init_subscriber(std::string base_name, f_void_f callback);

	// set a (new) messages to be sent
	void setProviderMsgs(json_object *pJsonSenML);
	void sendGETRequest();
	void sendPOSTRequest(json_object *data);

	//Overload - ApplicationServiceInterface callback
	int Callback_Serve_HTTP_GET(const char *Id, std::string *pStr);
	int Callback_Serve_HTTPs_GET(const char *Id, std::string *pStr,
				std::string sToken, std::string sSignature, std::string
				clientDistName);
	int Callback_Serve_HTTP_POST(const char *url, const char *payload);
	// Overload - OrchestrationInterface callback
	size_t Callback_OrchestrationResponse(char *ptr, size_t size);
	size_t callback_GET(const char *prt, size_t size);


private:
    
	/*Sensor registration, deregistration --- ApplicationService functions*/
	// create a access point for the rest of the system
	// register at serviceRegistry
	bool registerSensor();
	bool deregisterSensor(std::string _sensorURI);
	
	void init_Application();
	void init_W_orch();

	f_void_f callback; // callback for POST/GET-return
	std::string provider_uri;
	std::string base_name;
	bool sensorIsRegistered;
	json_object *msgs;
};
}
