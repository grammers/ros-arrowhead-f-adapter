#pragma once

#include <inttypes.h>
#include <map>
#include <mutex>
#include "../Interface/ApplicationServiceInterface.hpp"

class SensorHandler : ApplicationServiceInterface
{
public:
	SensorHandler();
	~SensorHandler();

	// configuration struct containing all values for setting up the procumer
	Arrowhead_Data_ext config;

	// initialisation cat be dun in constructor because the values are sett later.
	void initSensorHandler(std::string baseName);

	// set a (new) messages to be sent
	void processProvider(json_object *pJsonSenML);

	//Overload - ApplicationServiceInterface callback
	int Callback_Serve_HTTP_GET(const char *Id, std::string *pStr);
	int Callback_Serve_HTTPs_GET(const char *Id, std::string *pStr, std::string sToken, std::string sSignature, std::string clientDistName);


	/*Sensor registration, deregistration --- ApplicationService functions*/
	// create a access point for the rest of the system
	// register at serviceRegistry
	bool registerSensor();
	bool deregisterSensor(std::string _sensorURI);

private:
    bool sensorIsRegistered;
    std::string baseName;
	json_object *lastMeasuredValue;
};

// used in https
template<typename T>
vector<T>
split(const T &str, const T &delimiters) {
	vector<T> v;

	typename T::size_type start = 0;

	auto pos = str.find_first_of(delimiters, start);

	while (pos != T::npos) {
		if (pos != start)
			v.emplace_back(str, start, pos - start);
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}

	if (start < str.length()) {
		v.emplace_back(str, start, str.length() - start);
	}

	return v;
}
