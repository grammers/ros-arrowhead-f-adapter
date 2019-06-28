#include "messages.hpp"

std_msgs::Float32 Converter::temperature;

Converter::Converter(){
}
Converter::~Converter(){
}

// the recived msgs:
// 	{
// 		"e":[{
// 			"n": "this_is_the_sensor_id",
// 			"v":39.0,
// 			"t": "1561633848"}],
// 		"bn": "100",
// 		"bu": "Celsius"
// 	}

void Converter::parce(char *ptr){
	// parsuing theperature from json sneml respons	
	std::string str(ptr);
	struct json_object *obj;
	obj = json_tokener_parse(str.c_str()); //extrat a json objekt
	
	// relevant data is in a nested array
	struct json_object *e;
	json_object_object_get_ex(obj, "e", &e);

	// get intresting data from json array object
	// the array has lenth 1 but contains a json objekt
	struct json_object *v;
	json_object_object_get_ex(
	json_object_array_get_idx(e,0), "v", &v);
	temperature.data  = json_object_get_double(v);	
}
