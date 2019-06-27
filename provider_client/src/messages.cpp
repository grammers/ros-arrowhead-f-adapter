#include "messages.hpp"
//#include "std_msgs::Float32.h"

std_msgs::Float32 Converter::temperature;
//Converter::Converter(float init_temp){
//	temperature.data = init_temp;
//}
Converter::Converter(){
}
Converter::~Converter(){
}

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
	//double temp  = json_object_get_double(v);
	//temperature = 1.1;
}
