#include "OrchestratorInterface.hpp"
#include <iostream>

OrchestratorInterface::OrchestratorInterface( )
{

}

OrchestratorInterface::~OrchestratorInterface()
{
	deinit();
}

// Overload Sensor-handler callback functionality here!
size_t OrchestratorInterface::httpResponseCallback(char *ptr, size_t size)
{
	return Callback_OrchestrationResponse(ptr, size);
}

size_t OrchestratorInterface::httpsResponseCallback(char *ptr, size_t size)
{
	return Callback_OrchestrationResponse(ptr, size);
}

size_t OrchestratorInterface::Callback_OrchestrationResponse(char *ptr, size_t size)
{
	printf("Callback_OrchestrationResponse -- need to overwrite\n");
	return size;
}

bool OrchestratorInterface::init_OrchestratorInterface(Arrowhead_Data_ext *config)
{
	if(config->THIS_ADDRESS.size() != 0){
	    URI = "http://" + config->THIS_ADDRESS + ":" + to_string(config->THIS_PORT);

	    if ( MakeServer(config->THIS_PORT) ) {
		fprintf(stderr, "Error: Unable to start HTTP Server (%s:%d)!\n", config->THIS_ADDRESS.c_str(), config->THIS_PORT);
		return false;
	    }

	    printf("\nOrchestratorInterface started - %s:%d\n", config->THIS_ADDRESS.c_str(), config->THIS_PORT);

	}
	else{
	    printf("Warning: Could not parse IPv4 address from config, trying to use IPv6!\n");
	    URI = "http://" + config->THIS_ADDRESS6 + ":" + to_string(config->THIS_PORT);

	    if ( MakeServer(config->THIS_PORT) ) {
	    fprintf(stderr, "Error: Unable to start HTTP Server (%s:%d)!\n", config->THIS_ADDRESS6.c_str(), config->THIS_PORT);
	    return false;
	    }

	    printf("\nOrchestratorInterface started - %s:%d\n", config->THIS_ADDRESS6.c_str(), config->THIS_PORT);
	}

	return true;
}

int OrchestratorInterface::deinit( )
{
	Unload_IniFile();

	KillServer();

    return 0;
}

dictionary *OrchestratorInterface::Load_IniFile(char *fname)
{
	pini = iniparser_load(fname);
	if (pini)
		iniparser_dump(pini, NULL);

	return pini;
}

int OrchestratorInterface::Unload_IniFile()
{
	if (pini)
	{
		iniparser_freedict(pini);
		pini = NULL;
		return 0;
	}
	return 1;
}

int OrchestratorInterface::sendOrchestrationRequest(string requestForm, Arrowhead_Data_ext *config)
{
	if(config->SECURE_ARROWHEAD_INTERFACE)
          return SendHttpsRequest(requestForm, config->ACCESS_URI_HTTPS, "POST");
	else
          return SendRequest(requestForm, config->ACCESS_URI, "POST");
}
