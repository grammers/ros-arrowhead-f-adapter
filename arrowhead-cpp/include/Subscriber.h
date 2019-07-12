#pragma once

#include "ArrowheadDataExt.h"
#include "ApplicationServiceInterface.h"

/*
 * These class is an access pint for arrowhead framework.
 * Use these class if you desire a subscriber.
 *
 * Usages:
 * 1 create an instance of the class.
 * 2 set the params in ArrowheadDataExt.
 * 3 call init(...) to initialise the subscriber and connect to a publisher
 * 4 reserve data in callback(...) (user define) when the publisher publish
 *
*/

namespace arrowhead{

using f_void_f = void(*)(const char*, const char*);

	class Subscriber : ApplicationServiceInterface{
	public:
		Subscriber();
		~Subscriber();

		ArrowheadDataExt config;

		void init(std::string base_name, f_void_f callback);
		
		// when a POST request is received
		// @override
		int callbackServerHttpPOST(const char *url, 
						const char *payload);
	private:
		std::string base_name;
		f_void_f callback;
	};
}
