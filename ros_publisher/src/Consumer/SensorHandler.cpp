#include "SensorHandler.h"

SensorHandler::SensorHandler(){
}

SensorHandler::~SensorHandler(void){

}

void SensorHandler::processConsumer(json_object *pJsonSenML) {
	json_object *obj = pJsonSenML;

	if(obj == NULL){
	    fprintf(stderr, "Error: Could not parse payload: %s\n", json_object_get_string(pJsonSenML));
	    return;
	}

	json_object *jBN;
	if(!json_object_object_get_ex(obj, "BaseName", &jBN)){
	    fprintf(stderr, "Error: received json does not contain bn field!\n");
	    return;
	}


     std::string requestForm;
     if(!oConsumedService.getRequestForm(requestForm, &config)){
          fprintf(stderr, "Error: Request Form is missing for \n");
          return;
     }

	printf("\nrequestForm: %s\n", requestForm.c_str());

     printf("Sending Orchestration Request: (%s)\n", config.SECURE_ARROWHEAD_INTERFACE ? "Secure Arrowhead Interface" : "Insecure Arrowhead Interface");
	
	 msg = pJsonSenML;

     int returnValue = sendOrchestrationRequest(requestForm, &config);
}


size_t SensorHandler::Callback_OrchestrationResponse(char *ptr, size_t size) {
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
          std::string sProviderURI = "https://" + sIPAddress + ":" + std::to_string(uPort) + "/" + sURI + "?token=" + token + "&signature=" + signature;

          sendHttpsRequestToProvider(sProviderURI);
     }
     else{
          std::string sProviderURI = "http://" + sIPAddress + ":" + std::to_string(uPort) + "/" + sURI;
          sendRequestToProvider(sProviderURI);
     }

	return size;
}

inline size_t providerHttpResponseHandler(char *ptr, size_t size, size_t nmemb, void *userdata){
	return ((SensorHandler *)userdata)->providerHttpResponseCallback(ptr, size*nmemb);
}

void SensorHandler::sendRequestToProvider(std::string _sProviderURI){

     printf("\nsendHttpRequestToProvider: %s\n", _sProviderURI.c_str());

     int http_code = 0;
     CURLcode res;
  	CURL *curl;
	struct curl_slist *headers = NULL;
  	string agent;

	curl_global_init(CURL_GLOBAL_ALL);

  	curl = curl_easy_init();

  	if(curl){
		agent = "libcurl/"+string(curl_version_info(CURLVERSION_NOW)->version);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str());

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_get_string(msg));
		curl_easy_setopt(curl, CURLOPT_URL, _sProviderURI.c_str());

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

          curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
  	}

     curl_global_cleanup();
}

void SensorHandler::sendHttpsRequestToProvider(std::string _sProviderURI){

     //printf("\nsendHttpsRequestToProvider: %s\n\n", _sProviderURI.c_str());
     printf("\nsendHttpsRequestToProvider\n");

     int http_code = 0;
     CURLcode res;
  	CURL *curl;
	struct curl_slist *headers = NULL;
  	string agent;

	curl_global_init(CURL_GLOBAL_ALL);

  	curl = curl_easy_init();

  	if(curl){
		agent = "libcurl/"+string(curl_version_info(CURLVERSION_NOW)->version);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str());

          curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &providerHttpResponseHandler);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

          //---------------HTTPS SECTION--------------------------------------------------------
          //
          //--verbose
          //if ( curl_easy_setopt(curl, CURLOPT_VERBOSE,        1L)            != CURLE_OK)
          //     printf("error: CURLOPT_VERBOSE\n");
          //--insecure
          if ( curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L)            != CURLE_OK)
               printf("error: CURLOPT_SSL_VERIFYPEER\n");
          if ( curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L)            != CURLE_OK)
               printf("error: CURLOPT_SSL_VERIFYHOST\n");
          //--cert
          if ( curl_easy_setopt(curl, CURLOPT_SSLCERT,        "keys/clcert.pem")  != CURLE_OK)
               printf("error: CURLOPT_SSLCERT\n");
          //--cert-type
          if ( curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE,    "PEM")         != CURLE_OK)
               printf("error: CURLOPT_SSLCERTTYPE\n");
          //--key
          if ( curl_easy_setopt(curl, CURLOPT_SSLKEY,         "keys/privkey.pem") != CURLE_OK)
               printf("error: CURLOPT_SSLKEY\n");
          //--key-type
          if ( curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE,     "PEM")         != CURLE_OK)
               printf("error: CURLOPT_SSLKEYTYPE\n");
          //--pass
          if ( curl_easy_setopt(curl, CURLOPT_KEYPASSWD,      "12345")       != CURLE_OK)
               printf("error: CURLOPT_KEYPASSWD\n");
          //--cacert
          if ( curl_easy_setopt(curl, CURLOPT_CAINFO,         "keys/cacert.pem")  != CURLE_OK)
               printf("error: CURLOPT_CAINFO\n");
          //
          //---------------END OF HTTPS SECTION-------------------------------------------------

		curl_easy_setopt(curl, CURLOPT_URL, _sProviderURI.c_str());

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

          curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
  	}

     curl_global_cleanup();
}

size_t SensorHandler::providerHttpResponseCallback(char *ptr, size_t size){
	deinit();
	return size;
}
