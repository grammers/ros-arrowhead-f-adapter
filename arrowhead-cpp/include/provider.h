#pragma once

#include "arrowhead_data_ext.hpp"
#include "ApplicationServiceInterface.hpp"

namespace arrowhead{

	class Provider : ApplicationServiceInterface{
	public:
		Arrowhead_Data_ext config;

		Provider();
		~Provider();
		
		void init(std::string base_name);
	
		// Set new msgs to be returned on a GET request
		void setMsgs(json_object *msgs);
	
		// when a GET request is received
		int callbackServerHttpGET(const char *id, 
						std::string *pResponse);

	private:
		std::string base_name;
		json_object *msgs;
	};
}
