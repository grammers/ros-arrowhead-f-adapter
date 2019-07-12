#pragma once

#include "ArrowheadDataExt.h"
#include "OrchestratorInterface.h"
#include <json-c/json.h>

/*
 * These class is a access point for arrowhead framework
 * Use these class if you desire a publisher
 *
 * Usages:
 * 1 create a instance of the class.
 * 2 set the params in ArrowheadDataExt
 * 3 call init(...) to initialise a publisher
 * 4 call publish to publish(...) a msgs.
 *
*/

namespace arrowhead{

	class Publisher : OrchestratorInterface{
	public:
		Publisher();
		~Publisher();

		ArrowheadDataExt config;

		bool init();
		void publish(json_object *msgs);

	private:
	};
}
