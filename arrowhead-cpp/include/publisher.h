#pragma once

#include "arrowhead_data_ext.hpp"
#include "OrchestratorInterface.hpp"
#include <json-c/json.h>

namespace arrowhead{

	class Publisher : OrchestratorInterface{
	public:
		Publisher();
		~Publisher();

		Arrowhead_Data_ext config;

		bool init();
		void publish(json_object *msgs);

	private:
	};
}
