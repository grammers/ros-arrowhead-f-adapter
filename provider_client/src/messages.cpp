#include "messages.hpp"

sensor_msgs::Temperature Converter::temperature;

Converter::Converter(){
}
Converter::~Converter(){
}

// the received msgs:
// 	{
// 		"e":[{
// 			"n": "this_is_the_sensor_id",
// 			"v":39.0,
// 			"t": "1561633848"}],
// 		"bn": "100",
// 		"bu": "Celsius"
// 	}

void Converter::parce(char *ptr){
	// parsing temperature from json SneML response	
	std::string str(ptr);
	struct json_object *obj;
	obj = json_tokener_parse(str.c_str()); //extract a json object
	
	// relevant data is in a nested array
	struct json_object *e;
	json_object_object_get_ex(obj, "e", &e);

	// get interesting data from json array object
	// the array has length 1 but contains a json object
	struct json_object *v;
	struct json_object *t;
	json_object_object_get_ex(
		json_object_array_get_idx(e,0), "v", &v);
	json_object_object_get_ex(
		json_object_array_get_idx(e,0), "t", &t);
	temperature.temperature  = json_object_get_double(v);	
	temperature.header.seq = json_object_get_int(t);
}
