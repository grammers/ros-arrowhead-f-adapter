
#include "SensorHandler.h"
#include <map>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <algorithm>
#include <time.h>
#include "../Security/RSASecurity.h"


SensorHandler::SensorHandler(){
}

SensorHandler::~SensorHandler(void){
}

void SensorHandler::initSensorHandler(std::string baseName){

	// test sow there in not an error in set up for applicationServiceInterface
	if (!init_ApplicationServiceInterface(config)){
		fprintf(stderr, "unable to init applictionServiceInterface");
	}
	SensorHandler::baseName = baseName;
	sensorIsRegistered = false;
	registerSensor(); // register in the service register
	return;
}

void SensorHandler::processProvider(json_object *pJsonSenML) {
	//////////////////////////////////
	// check sow it it a valid msgs //
	//////////////////////////////////
	if(pJsonSenML == NULL){
	    fprintf(stderr, "Error: Could not parse SenML: %s\n", json_object_get_string(pJsonSenML));
	    return;
	}

     json_object *jBN;
	if(!json_object_object_get_ex(pJsonSenML, "BaseName", &jBN)){
	    fprintf(stderr, "Error: received json does not contain BaseName field!\n");
	    return;
	}

	std::string bn = std::string(json_object_get_string(jBN));

	if(bn != baseName){
		fprintf(stderr, "baseNames don not match: %s != %s\n", bn.c_str(), baseName.c_str());
		return;
	}
	///////////////
	// check end //
	///////////////
	
	// is sensor is register is a new msgs set
	if (sensorIsRegistered) {
		lastMeasuredValue = pJsonSenML;
		printf("New measurement received from: %s\n", baseName.c_str());
		printf("LastValue updated.\n");
		return;
	}
}

/*
--
--
-- Provider
-- Registration, deregistration
--
*/
bool SensorHandler::registerSensor(){
	printf("\nREGISTRATION (%s, %s)\n\n", config.SECURE_PROVIDER_INTERFACE ? "Secure Provider" : "Insecure Provider", config.SECURE_ARROWHEAD_INTERFACE ? "Secure AHInterface" : "Insecure AHInterface");

	
	///////////
	// HTTPS //
	///////////
	if(config.SECURE_PROVIDER_INTERFACE){
		std::ifstream ifs(config.PUBLIC_KEY_PATH.c_str());
		std::string pubkeyContent( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );

		pubkeyContent.erase(0, pubkeyContent.find("\n") + 1);
		pubkeyContent = pubkeyContent.substr(0, pubkeyContent.size()-25);

        pubkeyContent.erase(std::remove(pubkeyContent.begin(), pubkeyContent.end(), '\n'), pubkeyContent.end());

        config.AUTHENTICATION_INFO = pubkeyContent;
	}
    /////////
	// end //
	/////////

	// try to register in arrowhead servisRegistry (called function in AppligcationInterFace)
	// return int for error checking
	int returnValue = registerToServiceRegistry(config);

	printf("%s Post sent (SenML baseName = %s)\n", config.SECURE_ARROWHEAD_INTERFACE? "HTTPs" : "HTTP", baseName.c_str());
	printf("%s Post return value: %d\n", config.SECURE_ARROWHEAD_INTERFACE? "HTTPs" : "HTTP", returnValue);

	if (returnValue == 201 /*Created*/){
          sensorIsRegistered = true;
		return true;
	}
	else{ // error handling

         printf("Already registered?\n");
		printf("Try re-registration\n");

		returnValue = unregisterFromServiceRegistry(config);

		if (returnValue == 200 /*OK*/ || returnValue == 204 /*No Content*/) {
			printf("Unregistration is successful\n");
		}
		else {
			fprintf(stderr, "Unregistration is unsuccessful\n");
			return false;
		}

		returnValue = registerToServiceRegistry(config);

		if (returnValue == 201 /*Created*/) {
               sensorIsRegistered = true;
               return true;
		}
		else {
			fprintf(stderr, "unsuccessful registration\n");
			return false; //unsuccessful registration
		}
	}
}

//Send HTTP PUT to ServiceRegistry
bool SensorHandler::deregisterSensor(std::string _baseName){

	int returnValue = unregisterFromServiceRegistry(config);

	if( returnValue == 200 /*OK*/ || returnValue == 204 /*No Content*/) {
		return true;
	}

	return false; //unsuccessful unregistration
}

/*
--
-- Called, when Consumer request arrives -- HTTP GET Request
--
*/
int SensorHandler::Callback_Serve_HTTP_GET(const char *URL, std::string *pResponse){
    printf("\nHTTP GET request received\n");

    printf("Received URL: %s\n", URL);
    std::string tmp = "/" + config.CUSTOM_URL;
	// test sow the request has arived at the correct place 
	if (strcmp(tmp.c_str(), URL) != 0) {
		fprintf(stderr, "Error: Unknown URL: %s\n", URL);
		return 1;
	}

     *pResponse = json_object_get_string(lastMeasuredValue);
     printf("Response:\n%s\n\n", pResponse->c_str());

	return 1;
}

/*
--
-- Called, when Consumer request arrives -- HTTPs GET Request
--
*/
int SensorHandler::Callback_Serve_HTTPs_GET(const char *URL, std::string *pResponse, std::string _sToken, std::string _sSignature, std::string _clientDistName){
	printf("\nHTTPs GET request received\n");

     printf("Received URL: %s\n", URL);
     std::string tmp = "/" + config.CUSTOM_URL;
	if (strcmp(tmp.c_str(), URL) != 0) {
		printf("Error: Unknown URL: %s\n", URL);
		return 1;
	}

//Set the own private key path for RSA algorithms

     RSASecurity oRSASecurity;
	oRSASecurity.privateKeyPath = config.PRIVATE_KEY_PATH;

	if(oRSASecurity.privateKeyPath.size() == 0){
	    printf("Error: Unknown Provider Private Key File Path\n");
	    return 1;
	}

//We have to replace the 0x20 ascii code (space) with 0x2B (plus), before base64 decryption

     if( strstr(_sToken.c_str(), " ") != NULL )
          replace(_sToken.begin(),     _sToken.end(),     ' ', '+');

     if( strstr(_sSignature.c_str(), " ") != NULL )
          replace(_sSignature.begin(), _sSignature.end(), ' ', '+');

	oRSASecurity.sB64EncodedRSAEncryptedToken     = _sToken;
	oRSASecurity.sB64EncodedSignature             = _sSignature;

//Verify RSA signature

	if(oRSASecurity.getVerificationResult()){
	    printf("Successful RSA Signature verification\n");
	}
	else{
	    printf("Error: Unsuccessful RSA Signature verification - Wrong signature?\n");
	    return 1;
	}

//Parse the base64 decrypted token information, which should be a json string

     printf("\nRaw token info:\n%s\n\n", oRSASecurity.getDecryptedToken().c_str());

         json_object *obj = json_tokener_parse(oRSASecurity.getDecryptedToken().c_str());

    if(obj == NULL){
	fprintf(stderr, "Error: Could not parse token: \n%s\n", oRSASecurity.getDecryptedToken().c_str());
	return false;
    }

    json_object *jS;
    if(!json_object_object_get_ex(obj, "s", &jS)){
	fprintf(stderr, "Error: could not find s in Token\n");
	return false;
    }

    json_object *jC;
    if(!json_object_object_get_ex(obj, "c", &jC)){
	fprintf(stderr, "Error: could not find c in Token\n");
	return false;
    }

    json_object *jE;
    if(!json_object_object_get_ex(obj, "e", &jE)){
	fprintf(stderr, "Error: could not find e in Token\n");
	return false;
    }

    std::string service            = json_object_get_string(jS);
    std::string consumerCommonName = json_object_get_string(jC);
    uint64_t expired               = (uint64_t)json_object_get_int64(jE); //json_object_get_int64

//check s - format: interface.serviceDefinition
//
/*
     std::string serviceInterface;
     if(!oProvidedService.getServiceInterface(serviceInterface)){
          printf("Error: Could not get Provider's Service Interface\n");
     }

     std::string serviceDefinition;
     if(!oProvidedService.getServiceDefinition(serviceDefinition)){
          printf("Error: Could not get Provider's Service Definition\n");
     }

     std::string expectedService = serviceInterface + (std::string)"." + serviceDefinition;

     if(strcmp(service.c_str(), expectedService.c_str()) != 0){
          printf("Error: s (%s) parameter from Raw token info is not equal to the expected %s value\n", service.c_str(), expectedService.c_str() );
          return 1;
     }
     else{
          printf("service identification is successful\n");
     }
*/
//check c - consumer certification common name
//
//Example:
//client distinguished name: C=HU,CN=client1.testcloud1.aitia.arrowhead.eu
//consumerCommonName : client1.SmartGrid.SmartGridOperator

     vector<string> dn_content   = split<string>(_clientDistName, ",");
     vector<string> clientCN_v   = split<string>(dn_content[1], ".");
     vector<string> rawTokenCN_v = split<string>(consumerCommonName, ".");

     if(strcmp( clientCN_v[0].substr( 3, clientCN_v[0].size() ).c_str(), rawTokenCN_v[0].c_str() ) != 0){
          printf("Error: Client CN (%s) is not equal to raw token CN (%s)\n", clientCN_v[0].substr( 3, clientCN_v[0].size() ).c_str(), rawTokenCN_v[0].c_str() );
          return 1;
     }

//check e - expiration time
	if(expired != 0){
          time_t linuxEpochTime = std::time(0);

          if(expired < (uint64_t)linuxEpochTime){
               printf("Error: Expired time(%lu) is smaller than linux epoch time(%lu)!", expired, (uint64_t)linuxEpochTime);
               return 1;
          }
	}

     *pResponse = json_object_get_string(lastMeasuredValue);
     printf("Response:\n%s\n\n", pResponse->c_str());

	return 1;
}
