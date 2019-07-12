#pragma once

#include "ArrowheadDataExt.h"
#include "OrchestratorInterface.h"

/*
 * These class is a access point to arrowhead framework
 * Use these when you desire a Consumer.
 *
 * Usages:
 * 1 create a instance of these object
 * 2 set the params in ArrowheadDataExt
 * 3 call init(...) to start up the system.
 * 4 call request() when you wanna get data from a provider.
 *
 * 5 request response triggers a call to callback(...) (user implemented)
*/
namespace arrowhead{

	// for callbacks
	using f_void_f = void(*)(const char*, const char*);
	
	class Consumer : OrchestratorInterface{
	public:
		Consumer();
		~Consumer();
	
		ArrowheadDataExt config;

		bool init(f_void_f callback);

		void request();
		
		// @override
		size_t callbackRequest(const char *payload, size_t size);

	private:
		f_void_f callback;
	};

}
