#include "ConsumedService.h"

ConsumedService::ConsumedService() {
}

ConsumedService::~ConsumedService() {

}
bool ConsumedService::getRequestForm(std::string &rRequestForm, Arrowhead_Data_ext *config) {

		//TODO change these to a actual json implementation
		// change return path to handle json
		// cascade throe sensorHandler, OrchestraIntreface, http...
	rRequestForm = 
			"{ \"requesterSystem\": { "
				"\"systemName\": \""+config->THIS_SYSTEM_NAME+"\", "
				"\"address\": \""+config->THIS_ADDRESS+"\", "
				"\"port\": "+std::to_string(config->THIS_PORT)+", "
				"\"authenticationInfo\": \""+config->AUTHENTICATION_INFO+"\" }, "
			"\"requestedService\": { "
				"\"serviceDefinition\": \""+config->SERVICE_DEFINITION+"\", "
				"\"interfaces\": [ \""+config->INTERFACE+"\" ], "
				"\"serviceMetadata\": { "
					"\"security\": \""+config->SECURITY+"\" } }, "
			"\"orchestrationFlags\": { "
				"\"overrideStore\": "+std::to_string(config->OVERRIDE_STORE)+"," 
				"\"matchmaking\": "+std::to_string(config->MATCHMAKING)+", "
				"\"metadataSearch\": "+std::to_string(config->METADATA_SEARCH)+", "
				"\"pingProviders\": "+std::to_string(config->PING_PROVIDERS)+", "
				"\"onlyPreferred\": "+std::to_string(config->ONLY_PREFERRED)+", "
				"\"externalServiceRequest\": "+std::to_string(config->EXTERNAL_SERVICE_REQUEST)+" }, "
			"\"preferredProviders\": [ { "
			"\"providerSystem\": { "
				"\"systemName\": \""+config->TARGET_SYSTEM_NAME+"\", "
				"\"address\": \""+config->TARGET_ADDRESS+"\", "
				"\"port\": \""+std::to_string(config->TARGET_PORT)+"\" } } ] }";

	return true;
}
