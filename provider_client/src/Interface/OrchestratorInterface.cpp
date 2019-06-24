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

bool OrchestratorInterface::init_OrchestratorInterface()
{
	if(CLIENT_ADDRESS.size() != 0){
	    URI = "http://" + CLIENT_ADDRESS + ":" + to_string(CLIENT_PORT);

	    if ( MakeServer(CLIENT_PORT) ) {
		printf("Error: Unable to start HTTP Server (%s:%d)!\n", CLIENT_ADDRESS.c_str(), CLIENT_PORT);
		return false;
	    }

	    printf("\nOrchestratorInterface started - %s:%d\n", CLIENT_ADDRESS.c_str(), CLIENT_PORT);

	}
	else{
	    printf("Warning: Could not parse IPv4 address from config, trying to use IPv6!\n");
	    URI = "http://" + CLIENT_ADDRESS6 + ":" + to_string(CLIENT_PORT);

	    if ( MakeServer(CLIENT_PORT) ) {
	    printf("Error: Unable to start HTTP Server (%s:%d)!\n", CLIENT_ADDRESS6.c_str(), CLIENT_PORT);
	    return false;
	    }

	    printf("\nOrchestratorInterface started - %s:%d\n", CLIENT_ADDRESS6.c_str(), CLIENT_PORT);
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

int OrchestratorInterface::sendOrchestrationRequest(string requestForm, bool _bSecureArrowheadInterface)
{
	if(_bSecureArrowheadInterface)
          return SendHttpsRequest(requestForm, OR_BASE_URI_HTTPS, "POST");
	else
          return SendRequest(requestForm, OR_BASE_URI, "POST");
}
