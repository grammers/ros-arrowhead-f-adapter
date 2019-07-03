#include "messages.hpp"

std_msgs::Float32 Converter::temperature;

Converter::Converter(){
}
Converter::~Converter(){
}

void Converter::init(std::string sid, std::string du, std::string id){
	sensor_id = sid;
	data_unit = du;
	identety = id;
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
	json_object_object_get_ex(
		json_object_array_get_idx(e,0), "v", &v);
	temperature.data  = json_object_get_double(v);	
}

void Converter::set(double temp, int time){
	obj = json_object_new_object();	
	json_object *arr_obj = json_object_new_array();
	json_object *arr_cont = json_object_new_object();

	json_object_object_add(arr_cont,"ID", json_object_new_string(sensor_id.c_str()));
	json_object_object_add(arr_cont,"Temperature", json_object_new_double(temp));
	json_object_object_add(arr_cont,"Time_stamp", json_object_new_int(time));
	json_object_array_add(arr_obj, arr_cont);
	json_object_object_add(obj,"Entity", arr_obj);
	json_object_object_add(obj,"bn", json_object_new_string(identety.c_str()));
	json_object_object_add(obj,"Unit", json_object_new_string(data_unit.c_str()));

	return;
}

std::string Converter::getJsonMsgs(){
	return json_object_get_string(obj);
}
