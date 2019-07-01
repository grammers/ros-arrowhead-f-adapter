
#include "ProvidedService.h"

ProvidedService::ProvidedService(){
     printService();
     fillMetadata();
}

ProvidedService::~ProvidedService(){

}

void ProvidedService::printService() {
/*
		printf("\n-----------------------------\nProvidedService\n-----------------------------\n");
     printf("%s : %s\n", "Custom URL", customURL.c_str());
     printf("%s : %s\n", "System name", systemName.c_str());
     printf("%s : %s\n", "Service definition", serviceDefinition.c_str());
     printf("%s : %s\n", "Service interface", serviceInterface.c_str());
     printf("%s : %s\n", "Private key path", privateKeyPath.c_str());
     printf("%s : %s\n", "Public key path", publicKeyPath.c_str());
*/	printf("Meta values:\n");
     for(std::map<std::string,std::string>::iterator it = metadata.begin(); it!=metadata.end(); ++it )
          printf("%s : %s\n", it->first.c_str(), it->second.c_str());
 
}

void ProvidedService::fillMetadata(){
     metadata.insert(std::pair<std::string, std::string>("unit", "Celsius"));
     metadata.insert(std::pair<std::string, std::string>("security", "token")); //not used, when the provider is unsecure

//todo:
//add new/custom meta data
     //metadata.insert(std::pair<std::string, std::string>("customKey", "customValue"));
}

bool ProvidedService::getCustomURL(std::string &r) {
     if(CUSTOM_URL.length() == 0)
          return false;

     r = CUSTOM_URL;
	return true;
}

bool ProvidedService::getSystemName(std::string &r) {
     if(PROVIDER_SYSTEM_NAME.length() == 0)
          return false;

     r = PROVIDER_SYSTEM_NAME;
	return true;
}

bool ProvidedService::getServiceDefinition(std::string &r) {
     if(SERVICE_DEFINITION.length() == 0)
          return false;

     r = SERVICE_DEFINITION;
	return true;
}

bool ProvidedService::getServiceInterface(std::string &r) {
     if(INTERFACE.length() == 0)
          return false;

     r = INTERFACE;
	return true;
}

bool ProvidedService::getPrivateKeyPath(std::string &r) {
     if(PRIVATE_KEY_PATH.length() == 0)
          return false;

     r = PRIVATE_KEY_PATH;
          return true;
}

bool ProvidedService::getPublicKeyPath(std::string &r) {
     if(PUBLIC_KEY_PATH.length() == 0)
          return false;

     r = PUBLIC_KEY_PATH;
          return true;
}

