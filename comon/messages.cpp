#include "messages.hpp"

sensor_msgs::Temperature Converter::temperature;

Converter::Converter(){
}
Converter::~Converter(){
}

// these initial set up can't be don in the constructor dud to where it is declared and where the params that are used to set it up is declared.
void Converter::init(std::string sensor_id, std::string unit, std::string baseName){
	Converter::sensor_id = sensor_id;
	data_unit = unit;
	identety = baseName;
}


// the msgs:
// 	{
// 		"Entity":[{
// 			"ID": "this_is_the_sensor_id",
// 			"Temperature":39.0,
// 			"Time_stamp": "1561633848"}],
// 		"BaseName": "100",
// 		"Unit": "Celsius"
// 	}

void Converter::parce(const char *url, const char *ptr){
	printf("\nin side parce\n");
	// parsing temperature from json SneML response	
	std::string str(ptr);
	struct json_object *obj;
	obj = json_tokener_parse(str.c_str()); //extract a json object
	
	// relevant data is in a nested array
	struct json_object *entity;
	json_object_object_get_ex(obj, "Entity", &entity);

	// get time stamp from from msgs
	// the array has length 1 but contains a json object
	struct json_object *time;
	json_object_object_get_ex(
		json_object_array_get_idx(entity,0), "Time_stamp", &time);
	

	// Check sow it is a new measurement and not an old
	int TIME = json_object_get_int(time);
	if (TIME > temperature.header.stamp.sec){
		temperature.header.stamp.sec  = TIME;
		
		// get interesting data from json array object
		// the array has length 1 but contains a json object
		struct json_object *temp;
		json_object_object_get_ex(
			json_object_array_get_idx(entity,0), "Temperature", &temp);
		temperature.temperature  = json_object_get_double(temp);	

		printf("new temperature received: %f\n", temperature.temperature);
	}
}


// build the msgs that are sent.
void Converter::set(double temp, int time){
	obj = json_object_new_object();	
	json_object *arr_obj = json_object_new_array();
	json_object *arr_cont = json_object_new_object();

	json_object_object_add(arr_cont,"ID", json_object_new_string(sensor_id.c_str()));
	json_object_object_add(arr_cont,"Temperature", json_object_new_double(temp));
	json_object_object_add(arr_cont,"Time_stamp", json_object_new_int(time));
	json_object_array_add(arr_obj, arr_cont);
	json_object_object_add(obj,"Entity", arr_obj);
	json_object_object_add(obj,"BaseName", json_object_new_string(identety.c_str()));
	json_object_object_add(obj,"Unit", json_object_new_string(data_unit.c_str()));

	return;
}

void Converter::updateMsgs(){
	set(temperature.temperature, temperature.header.stamp.sec);
}


// return the msgs
json_object* Converter::getJsonMsgs(){
	return obj;
}
