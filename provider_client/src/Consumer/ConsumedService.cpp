#include "ConsumedService.h"

ConsumedService::ConsumedService() {
}

ConsumedService::~ConsumedService() {

}
bool ConsumedService::getRequestForm(std::string consumerID, std::string &rRequestForm) {

	if (CONSUMER_ID == consumerID){
		//TODO change these to a actual json implementation
		// change return path to handle json
		// caskade thrue sensorHandler, OrchestraIntreface, http...
		rRequestForm = 
				"{ \"requesterSystem\": { "
					"\"systemName\": \""+CLIENT_SYSTEM_NAME+"\", "
					"\"address\": \""+CLIENT_ADDRESS+"\", "
					"\"port\": "+std::to_string(CLIENT_PORT)+", "
					"\"authenticationInfo\": \""+AUTHENTICATION_INFO+"\" }, "
				"\"requestedService\": { "
					"\"serviceDefinition\": \""+SERVICE_DEFINITION+"\", "
					"\"interfaces\": [ \""+INTERFACES+"\" ], "
					"\"serviceMetadata\": { "
						"\"security\": \""+SECURITY+"\" } }, "
				"\"orchestrationFlags\": { "
					"\"overrideStore\": "+std::to_string(OVERRIDE_STORE)+"," 
					"\"matchmaking\": "+std::to_string(MATCHMAKING)+", "
					"\"metadataSearch\": "+std::to_string(METADATA_SEARCH)+", "
					"\"pingProviders\": "+std::to_string(PING_PROVIDERS)+", "
					"\"onlyPreferred\": "+std::to_string(ONLY_PREFERRED)+", "
					"\"externalServiceRequest\": "+std::to_string(EXTERNAL_SERVICE_REQUEST)+" }, "
				"\"preferredProviders\": [ { "
				"\"providerSystem\": { "
					"\"systemName\": \""+PROVIDER_SYSTEM_NAME+"\", "
					"\"address\": \""+PROVIDER_ADDRESS+"\", "
					"\"port\": \""+std::to_string(PROVIDER_PORT)+"\" } } ] }";

		return true;
	}
	return false;
}
