#pragma once

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <list>

#include "Http_Handler.hpp"
#include "Https_Handler.hpp"
#include "../Consumer/arrowhead_data_ext.hpp"

#ifdef __linux__
#include "ini/iniparser.h"
#elif _WIN32
extern "C" {
#include "ini/iniparser.h"
}
#endif

class OrchestratorInterface : Http_Handler, Https_Handler {

private:

	dictionary  *pini = NULL;
	string URI;

	dictionary *Load_IniFile(char *fname);
	int Unload_IniFile();

public:
	string sConsumerID;

	OrchestratorInterface();
	~OrchestratorInterface();

	bool init_OrchestratorInterface(Arrowhead_Data_ext *config);
	int deinit();

	int sendOrchestrationRequest(string rResult, Arrowhead_Data_ext *config);

	size_t httpResponseCallback(char *ptr, size_t size);
	size_t httpsResponseCallback(char *ptr, size_t size);

	virtual size_t Callback_OrchestrationResponse(char *ptr, size_t size);
};
