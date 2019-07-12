#pragma once

#include "ArrowheadDataExt.h"
#include "ApplicationServiceInterface.h"
#include <json-c/json.h>

/*
 * These class is an access point to arrowhead framework.
 * Use these class when you desire a provider.
 *
 * Usages:
 * 1 create an instance of the class.
 * 2 set the params in ArrowheadDataExt.
 * 3 call init(...) to initialise the provider.
 * 4 call setMsgs(...) to set the msgs that will be returned,
 * when a consumer request it.
*/


namespace arrowhead{

	class Provider : ApplicationServiceInterface {
	public:
		ArrowheadDataExt config;

		Provider();
		~Provider();
		
		void init(std::string base_name);
	
		// Set new msgs to be returned on a GET request
		void setMsgs(json_object *msgs);
	
		// when a GET request is received
		// @override
		int callbackServerHttpGET(const char *id, 
						std::string *data_str);

	private:
		std::string base_name;
		json_object *msgs;
	};
}
