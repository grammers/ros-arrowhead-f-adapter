#pragma once

#include <string>
#include <map>

class ProvidedService {
private:
// todo:
// modify these parameters
     std::string customURL         = "130.240.5.68";
     std::string systemName        = "subscriber_ah_insecure";
     std::string serviceDefinition = "example_temperature";
     std::string serviceInterface  = "REST-JSON-SENML";
     std::string privateKeyPath    = "keys/tempsensor.testcloud1.private.key";
     std::string publicKeyPath     = "keys/tempsensor.testcloud1.publickey.pem";

public:
//do not modify below this
     ProvidedService();
     ~ProvidedService();

     std::map<std::string, std::string> metadata;
     void fillMetadata();

     void printService();
     bool getCustomURL(std::string &r);
     bool getSystemName(std::string &r);
     bool getServiceDefinition(std::string &r);
     bool getServiceInterface(std::string &r);
     bool getPrivateKeyPath(std::string &r);
     bool getPublicKeyPath(std::string &r);
};
