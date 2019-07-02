#pragma once

#include <inttypes.h>
#include <string>
#include <map>
#include <mutex>
#include "../Interface/ApplicationServiceInterface.hpp"
//#include "../global_const.hpp"
//#include "ProvidedService.h"

class SensorHandler : ApplicationServiceInterface
{
public:
	SensorHandler();
	~SensorHandler();

	void initSensorHandler();

	void processProvider(std::string pJsonSenML);

	//Overload - ApplicationServiceInterface callback
	int Callback_Serve_HTTP_GET(const char *Id, string *pStr);
	int Callback_Serve_HTTPs_GET(const char *Id, string *pStr, string sToken, string sSignature, string clientDistName);

	Arrowhead_Data_ext config;

     bool sensorIsRegistered;

     std::string baseName;
     std::string baseUnit;
	double value;
	std::string lastMeasuredValue;
     std::string meta_unit;

	/*ProvidedServices*/
	//ProvidedService oProvidedService;

	/*Sensor registration, deregistration --- ApplicationService functions*/
	bool registerSensor(std::string _jsonSenML);
	bool deregisterSensor(std::string _sensorURI);

};

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