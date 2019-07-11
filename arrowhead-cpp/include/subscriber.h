#pragma once

#include "arrowhead_data_ext.hpp"
#include "ApplicationServiceInterface.hpp"

namespace arrowhead{

using f_void_f = void(*)(const char*, const char*);

	class Subscriber : ApplicationServiceInterface{
	public:
		Subscriber();
		~Subscriber();

		Arrowhead_Data_ext config;

		void init(std::string base_name, f_void_f callback);
		
		// when a POST request is received
		int callbackServerHttpPOST(const char *url, 
						const char *payload);
	private:
		std::string base_name;
		f_void_f callback;
	};
}
