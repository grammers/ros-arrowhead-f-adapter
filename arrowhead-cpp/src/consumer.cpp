#include "consumer.h"

namespace arrowhead{

	Consumer::Consumer(){}
	Consumer::~Consumer(){}

	bool Consumer::init(f_void_f callback){
		std::string request_form;
		if(!getOrchetrationRequestForm(request_form, config)){
			fprintf(stderr, "Error: Request form is missing");
			return false;
		}
	
		// NOTE might put in orchestration instead
		printf("\nrequest form: %s\n", request_form.c_str());
		printf("Sending Orchestration Request to: %s\n", 
						config.SECURE_ARROWHEAD_INTERFACE ? 
							"Secure Arrowhead Interface" : 
							"Insecure Arrowhead Interface");
	
		//fprintf(stderr, "\nreturn nr: %i\n", sendOrchestrationRequest(request_form, &config));
		if(200 != sendOrchestrationRequest(request_form, &config)){
			fprintf(stderr, "can't connect to provider\n");
			return false;
		}
		
		Consumer::callback = callback;
		return true;
	}

	// sends a GET request to provider
	// callback arrives at callbackRequest
	void Consumer::request(){
		printf("Sending GET request to: %s\n", 
						config.TARGET_SYSTEM_NAME.c_str());
		sendRequestToProvider("", this -> target_uri, "GET"); 
	}

	size_t Consumer::callbackRequest(const char *payload, size_t size){
		printf("Request received data: %s\n", payload);
		// figure out somtheing good to send ass uri
		this -> callback(config.SERVICE_URI.c_str(), payload);
		return size;
	}



}
