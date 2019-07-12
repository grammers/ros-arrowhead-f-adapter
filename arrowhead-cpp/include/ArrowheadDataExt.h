#pragma once
#include <iostream>

// Convert boolean ("x") to string "true"/"false"
// required to print booleans ass string
#define b2s(x) ((x)?"true":"false")

// these params need to be set
// they have to agree whit arrowhead core to work

namespace arrowhead{
	// struct contains all configure variables
	struct ArrowheadDataExt {
		// service settings
		std::string SERVICE_DEFINITION;
		std::string INTERFACE;
		std::string	SERVICE_URI;
		std::string UNIT;
		std::string SECURITY;
	
		// connect to core
		std::string ACCESS_URI;
		std::string ACCESS_URI_HTTPS;
	
		// orchestrator setting
		bool OVERRIDE_STORE;
		bool MATCHMAKING;
		bool METADATA_SEARCH;
		bool PING_PROVIDERS;
		bool ONLY_PREFERRED;
		bool EXTERNAL_SERVICE_REQUEST;

		// this procumer
		std::string	THIS_SYSTEM_NAME;
		std::string THIS_ADDRESS;
		std::string THIS_ADDRESS6;
		int THIS_PORT;

		// target procumer
		std::string TARGET_SYSTEM_NAME;
		std::string TARGET_ADDRESS;
		int TARGET_PORT;
	
		// security 
		bool SECURE_ARROWHEAD_INTERFACE;
		bool SECURE_PROVIDER_INTERFACE;
		std::string PUBLIC_KEY_PATH;
		std::string PRIVATE_KEY_PATH;
		std::string AUTHENTICATION_INFO;

		void print(){
			printf("The set params\n");
			printf("Service settings\n");
			printf("SERVICE_DEFINITION: %s\n", SERVICE_DEFINITION.c_str());
			printf("INTERFACE: %s\n", INTERFACE.c_str());
			printf("SERVICE_URI: %s\n", SERVICE_URI.c_str());
			printf("UNIT : %s\n", UNIT.c_str());
			printf("SECURITY: %s\n", SECURITY.c_str());
		
			printf("Core conection\n");
			printf("ACCESS_URI: %s\n", ACCESS_URI.c_str());
			printf("ACCESS_URI_HTTPS: %s\n", ACCESS_URI_HTTPS.c_str());
		
			printf("Orchestrator settings\n");
			printf("OVERRIDE_STORE: %s\n", b2s(OVERRIDE_STORE));
			printf("MATCHMAKING: %s\n", b2s(MATCHMAKING));
			printf("METADATA_SEARCH: %s\n", b2s(METADATA_SEARCH));
			printf("PING_PROVIDERS: %s\n", b2s(PING_PROVIDERS));
			printf("ONLY_PREFERRED: %s\n", b2s(ONLY_PREFERRED));
			printf("EXTERNAL_SERVICE_REQUEST: %s\n",
							b2s(EXTERNAL_SERVICE_REQUEST));

			printf("This procomer\n");
			printf("THIS_SYSTEM_NAME: %s\n", THIS_SYSTEM_NAME.c_str());
			printf("THIS_ADDRESS: %s\n", THIS_ADDRESS.c_str());
			printf("THIS_ADDRESS6: %s\n", THIS_ADDRESS6.c_str());
			printf("THIS_PORT: %i\n", THIS_PORT);
			
			printf("Target provomer\n");
			printf("TARGET_SYSTEM_NAME: %s\n", TARGET_SYSTEM_NAME.c_str());
			printf("TARGET_ADDRESS: %s\n", TARGET_ADDRESS.c_str());
			printf("TARGET_PORT: %i\n", TARGET_PORT);
			
			printf("securety\n");
			printf("PUBLIC_KEY_PATH: %s\n", PUBLIC_KEY_PATH.c_str());
			printf("PRIVATE_KEY_PATH: %s\n", PRIVATE_KEY_PATH.c_str());
			printf("AUTHENTICATION_INFO: %s\n", AUTHENTICATION_INFO.c_str());
			printf("SECURE_ARROWHEAD_INTERFACE: %s\n", 
							b2s(SECURE_ARROWHEAD_INTERFACE));
			printf("SECURE_PROVIDER_INTERFACE: %s\n", 
							b2s(SECURE_PROVIDER_INTERFACE));
		}
	};
}
