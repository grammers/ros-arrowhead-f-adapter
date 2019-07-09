
#include "arrowhead_cpp.h"
#include <map>
#include <mutex>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <algorithm>
#include <time.h>
#include "RSASecurity.h"


Arrowhead::Arrowhead(){
}

Arrowhead::~Arrowhead(void){
}

void Arrowhead::init_provider(std::string base_name){
	init_Application();

	sensorIsRegistered = false;
	Arrowhead::base_name = base_name;
	registerSensor(); // register in the service register
	return;
}

void Arrowhead::inti_subscriber(std::string base_name, f_void_f callback){
	
	init_Application();

	// callback function
	Arrowhead::callback = callback;

	sensorIsRegistered = false;
	Arrowhead::base_name = base_name;
	registerSensor(); // register in the service register
	return;
}

void Arrowhead::init_Application(){
	// test sow there in not an error in set up for applicationServiceInterface
	if (!init_ApplicationServiceInterface(config)){
		fprintf(stderr, "unable to init applictionServiceInterface");
	}
}


void Arrowhead::init_consumer(f_void_f callback){
	init_W_orch();	
	Arrowhead::callback = callback;
}

void Arrowhead::init_publisher(){
	init_W_orch();
}

void Arrowhead::init_W_orch(){
	std::string request_form;
	if(!getOrchetrationRequestForm(request_form)){
		fprintf(stderr, "Error: Request form is missing");
		return;
	}
	
	printf("\nrequest form: %s\n", request_form.c_str());
	printf("Sending Orchestration Request to: %s\n", config.SECURE_ARROWHEAD_INTERFACE ? "Secure Arrowhead Interface" : "Insecure Arrowhead Interface");

	sendOrchestrationRequest(request_form, &config);
}

void Arrowhead::setProviderMsgs(json_object *pJsonSenML) {
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

	if(bn != base_name){
		fprintf(stderr, "baseNames don not match: %s != %s\n", bn.c_str(), base_name.c_str());
		return;
	}
	///////////////
	// check end //
	///////////////
	
	// is sensor is register is a new msgs set
	if (sensorIsRegistered) {
		msgs = pJsonSenML;
		printf("New measurement received from: %s\n", base_name.c_str());
		printf("LastValue updated.\n");
		return;
	}
}

// sends a GET request to provider
// callback arrives at callback_GET
void Arrowhead::sendGETRequest(){
	printf("Sending GET request to: %s\n", config.TARGET_SYSTEM_NAME.c_str());
	sendRequestToProvider("", provider_uri, "GET");
}

// sends a POST request to provider
void Arrowhead::sendPOSTRequest(json_object *data){
	sendRequestToProvider(json_object_get_string(data), provider_uri, "POST");
}


/*
--
--
-- Provider
-- Registration, deregistration
--
*/
bool Arrowhead::registerSensor(){
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

	// try to register in Arrowhead servisRegistry (called function in AppligcationInterFace)
	// return int for error checking
	int returnValue = registerToServiceRegistry(config);

	printf("%s Post sent (SenML baseName = %s)\n", config.SECURE_ARROWHEAD_INTERFACE? "HTTPs" : "HTTP", base_name.c_str());
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
bool Arrowhead::deregisterSensor(std::string _base_name){

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
int Arrowhead::Callback_Serve_HTTP_GET(const char *URL, std::string *pResponse){
    printf("\nHTTP GET request received\n");

    printf("Received URL: %s\n", URL);
    std::string tmp = "/" + config.SERVICE_URI;
	// test sow the request has arived at the correct place 
	if (strcmp(tmp.c_str(), URL) != 0) {
		fprintf(stderr, "Error: Unknown URL: %s\n", URL);
		return 1;
	}

     *pResponse = json_object_get_string(msgs);
     printf("Response:\n%s\n\n", pResponse->c_str());

	return 1;
}

/*
--
-- Called, when Consumer request arrives -- HTTP POST Request
--
*/
int Arrowhead::Callback_Serve_HTTP_POST(const char *url, const char *payload){
	printf("POST received in Arrowhead.\npayload: %s\nurl: %s\n", payload, url);
	callback(url, payload);	
	return 1;
}

/*
--
-- Called, when Consumer request arrives -- HTTPs GET Request
--
*/
int Arrowhead::Callback_Serve_HTTPs_GET(const char *URL, std::string *pResponse, std::string _sToken, std::string _sSignature, std::string _clientDistName){
	printf("\nHTTPs GET request received\n");

     printf("Received URL: %s\n", URL);
     std::string tmp = "/" + config.SERVICE_URI;
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
//client distinguished name: C=HU,CN=client1.testcloud1.aitia.Arrowhead.eu
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

     *pResponse = json_object_get_string(msgs);
     printf("Response:\n%s\n\n", pResponse->c_str());

	return 1;
}


// override orchestration
// run when a GET request is sent
size_t Arrowhead::callback_GET(const char *ptr, size_t size){
	// figure out somtheing good to send ass uri
	callback(config.SERVICE_URI.c_str(), ptr);
	return size;
}


bool Arrowhead::getOrchetrationRequestForm(std::string &request_form){
	//TODO change these to a actual json implementation
    // change return path to handle json
    // cascade throe sensorHandler, OrchestraIntreface, http...
    request_form = 
    	"{ \"requesterSystem\": { "     
        	"\"systemName\": \""+config.THIS_SYSTEM_NAME+"\", "
            "\"address\": \""+config.THIS_ADDRESS+"\", "
            "\"port\": "+std::to_string(config.THIS_PORT)+", "
            "\"authenticationInfo\": \""+config.AUTHENTICATION_INFO+"\" }, "
		"\"requestedService\": { "      
        	"\"serviceDefinition\": \""+config.SERVICE_DEFINITION+"\", "
            "\"interfaces\": [ \""+config.INTERFACE+"\" ], "
            "\"serviceMetadata\": { "       
            	"\"security\": \""+config.SECURITY+"\" } }, "
		"\"orchestrationFlags\": { "    
        	"\"overrideStore\": "+std::to_string(config.OVERRIDE_STORE)+"," 
            "\"matchmaking\": "+std::to_string(config.MATCHMAKING)+", "
            "\"metadataSearch\": "+std::to_string(config.METADATA_SEARCH)+", "
            "\"pingProviders\": "+std::to_string(config.PING_PROVIDERS)+", "
            "\"onlyPreferred\": "+std::to_string(config.ONLY_PREFERRED)+", "
            "\"externalServiceRequest\": "+std::to_string(config.EXTERNAL_SERVICE_REQUEST)+" }, "
        "\"preferredProviders\": [ { "
        "\"providerSystem\": { "        
        	"\"systemName\": \""+config.TARGET_SYSTEM_NAME+"\", "
            "\"address\": \""+config.TARGET_ADDRESS+"\", "
            "\"port\": \""+std::to_string(config.TARGET_PORT)+"\" } } ] }";
 
     return true;

}

size_t Arrowhead::Callback_OrchetrationResponse(char *ptr, size_t size) {
//
//Expected Response -- example
//{
//  "response": [
//    {
//      "service": {
//        "serviceDefinition": "IndoorTemperature_ProviderExample",
//        "interfaces": [
//          "REST-JSON-SENML"
//        ],
//        "serviceMetadata": {
//          "unit": "Celsius"
//        }
//      },
//      "provider": {
//        "systemName": "SecureTemperatureSensor",
//        "address": "10.0.0.103",
//        "port": 8452,
//        "authenticationInfo": "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyzDRU+P6h8Jwp9eiGYqqlgoAmPLo6M/PTZX+pkKr2MIg7VLdnjUeXzKFljwJKjYGG3nus53F4RFnymT7VoIQT+SmkuLy90Ir6O3XRWiD74XlOIkthT8/fq5FP9sJIusaRc9jkx3Y8jC3yCz1BPJDa+0A+heWarN+K7W7985aBFiJ1ycsB7yJFYAt7wVRc2fkgGpmp4l34Ta4J7QVwzYBOx5w5hIE29EzXOhl0GB6c/licclhisOnN31OWizoWJWAdexmjR9ugHgFSv4eUbjQ3/Qc0tM3ljmbnMMmj54fKZHtpesLXrCi44aQ88e7UOd/xplAbntEPvz168oie4IzFQIDAQAB"
//      },
//      "serviceURI": "/1/1/REST-JSON-SENML",
//      "instruction": null,
//      "authorizationToken": "t3AeN6Q0xa177mV+9WkQlsLL3arBnNrYm/sZ/OJTSSrpGRBz3alaQT+0ZckJrg3brIE2ofzyzYs0wKxwQbW9T12T+hW0DeFyN3hmxTZk0Kaw9h56M9p4N6srlid8Ewu5qesPLQLbQ1AF3Q7HCy7uKIbDjTeGdd1IZmae/QP8CjhnVRu1uZreiFVzCf9HUsQpfnRbssrjtXkmvcWXbxx1BD4mFCEI6Ze5CnTld+GSWUawM39byz5cQbq2cBrecKfGC5zQh1vHFvTxrinq5ARnO3S9Gaul463P0QO3zWEFehbIscvEwAC6rUx33NMjdTqdfXd7n8VdED0yMbWaQ86ApQ==",
//      "signature": "W6ODMMDD5xFlT3UycKBSsbJ5pskQds69nmiZ0cXj+KrblVdkoTsYRgzoFBodw+XLBf8MvDphfMDYqihjzPmq1xMj0v+AkEB6nV3uMTznR9cEW9VOgGBVZtRt+h9Sh5HNe2jRblLD7+kimmclCU5zqqHZEOrqShaZGuNEJeZg41eokuFBzARTWlckyM5kTM+GgalFtpMevGuqBd5FcpYVC1TNoMlABIYUqPzrHsKFsRifLSMeqLnvGmWZB/HKDDl2iQ+jPG58DcxdoqPcLAd+vzq/+oir0n/OHH1DFvdyjQ7NMIc8u1sz+uhm1jOQZlmgl2O5aPWDoD3mSb0Zduv2Nw=="
//    }
//  ]
//}
//
     printf("Orchestration response: %s\n", ptr);

     std::string token;
     std::string signature;
     string sIPAddress;
	uint32_t uPort;
	string sInterface;
	string sURI;

     struct json_object *obj = json_tokener_parse(ptr);
     if(obj == NULL){
          fprintf(stderr, "Error: could not parse orchestration response\n");
          return 1;
     }

     struct json_object *jResponseArray;
     if(!json_object_object_get_ex(obj, "response", &jResponseArray)){
          fprintf(stderr, "Error: could not parse response\n");
          return 1;
     }

     struct json_object *jResponse = json_object_array_get_idx(jResponseArray, 0);
     struct json_object *jProv2;

     if(!json_object_object_get_ex(jResponse, "provider", &jProv2)){
          fprintf(stderr, "Error: could not parse provider section\n");
          return 1;
     }

     struct json_object *jAddr;
     struct json_object *jPort;
     struct json_object *jService;
     struct json_object *jIntf;
     struct json_object *jIntf0;
     struct json_object *jUri;
     struct json_object *jToken;
     struct json_object *jSignature;

     if(!json_object_object_get_ex(jProv2,    "address",    &jAddr))    {fprintf(stderr, "Error: could not find address\n");    return 1;}
     if(!json_object_object_get_ex(jProv2,    "port",       &jPort))    {fprintf(stderr, "Error: could not find port\n");       return 1;}
     if(!json_object_object_get_ex(jResponse, "service",    &jService)) {fprintf(stderr, "Error: could not find service\n");    return 1;}
     if(!json_object_object_get_ex(jService,  "interfaces", &jIntf))    {fprintf(stderr, "Error: could not find interface\n");  return 1;}
     if(!json_object_object_get_ex(jResponse,  "serviceURI", &jUri))     {fprintf(stderr, "Error: could not find serviceURI\n"); return 1;}

     jIntf0 = json_object_array_get_idx(jIntf, 0);

     if(jIntf0 == NULL){
          fprintf(stderr, "Error: could not find interface\n");
          return 1;
     }

	sIPAddress = string(json_object_get_string(jAddr));
	uPort      = json_object_get_int(jPort);
	sInterface = string(json_object_get_string(jIntf0));
     sURI       = string(json_object_get_string(jUri));

	 if(config.SECURE_PROVIDER_INTERFACE){
     	if(!json_object_object_get_ex(jResponse,  "authorizationToken", &jToken)){
			fprintf(stderr, "Error: could not find authorizationToken\n");
			return 1;
		}
		if(!json_object_object_get_ex(jResponse,  "signature",          &jSignature)){
			fprintf(stderr, "Error: could not find signature\n");
			return 1;
		}

          token     = string(json_object_get_string(jToken));
          signature = string(json_object_get_string(jSignature));

          //https://ipaddr:port/serviceURI?token=_token_&signature=_signature_
          provider_uri = "https://" + sIPAddress + ":" + std::to_string(uPort) + "/" + sURI + "?token=" + token + "&signature=" + signature;
     }
     else{
           provider_uri= "http://" + sIPAddress + ":" + std::to_string(uPort) + "/" + sURI;
     }

	return size;
}
