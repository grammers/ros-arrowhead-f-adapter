#pragma once

#include "arrowhead_data_ext.hpp"
#include "OrchestratorInterface.hpp"

namespace arrowhead{

	// for callbacks
	using f_void_f = void(*)(const char*, const char*);
	
	class Consumer : OrchestratorInterface{
	public:
		Consumer();
		~Consumer();
	
		Arrowhead_Data_ext config;

		bool init(f_void_f callback);

		void request();

		size_t callbackRequest(const char *payload, size_t size);

	private:
		f_void_f callback;
	};

}
