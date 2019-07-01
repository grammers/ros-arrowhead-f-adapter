#pragma once

#include <string>
#include <map>
#include "../global_const.hpp"

class ProvidedService {
private:

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
