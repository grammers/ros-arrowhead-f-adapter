#include "Provider.h"

#ifdef __linux__
     #include "iniparser.h"
#elif _WIN32
     extern "C" {
     #include "iniparser.h"
     }
#endif

namespace arrowhead{
		
	Provider::Provider(){}
	Provider::~Provider(){}

	void Provider::init(std::string base_name) {
		// test sow there in not an error in set up for applicationServiceInterface
		if (!initApplicationServiceInterface(config))
			fprintf(stderr, "unable to init applictionServiceInterface");

		this -> base_name = base_name;
		
		// register in the service register
		if(!registerSensor(config, base_name))
				fprintf(stderr, "cod not register sensor!\n");
	
		return;
	}
	
	void Provider::setMsgs(json_object *msgs) {
		//////////////////////////////////
		// check sow it it a valid msgs //
		//////////////////////////////////
		if(msgs == NULL){
			fprintf(stderr, "Error: Could not parse SenML: %s\n", 
							json_object_get_string(msgs));
			return;
		}

		 json_object *jBN;
		if(!json_object_object_get_ex(msgs, "BaseName", &jBN)) {
			fprintf(stderr, "Error: received json does not contain BaseName field!\n");
			return;
		}

		std::string bn = std::string(json_object_get_string(jBN));

		if(bn != base_name) {
			fprintf(stderr, "baseNames don not match: %s != %s\n", 
							bn.c_str(), base_name.c_str());
			return;
		}
		///////////////
		// check end //
		///////////////
		
		// if sensor is register is a new msgs set
		if (sensorIsRegistered) {
			this -> msgs = msgs;
			printf("New measurement received from: %s\n", 
						this -> base_name.c_str());
			printf("LastValue updated.\n");
			return;
		}
	}


	// @override
	int Provider::callbackServerHttpGET(const char *URL, 
						std::string *data_stre) {
    	printf("\nHTTP GET request received\n");

  	 	printf("Received URL: %s\n", URL);
    	std::string tmp = "/" + this -> config.SERVICE_URI;
		// test sow the request has arived at the correct place 
		if (strcmp(tmp.c_str(), URL) != 0) {
			fprintf(stderr, "Error: Unknown URL: %s\n", URL);
			return 1;
		}
		
    	 *data_stre= json_object_get_string(msgs);
	     printf("Response:\n%s\n\n", data_stre->c_str());

		return 1;
	}

}
