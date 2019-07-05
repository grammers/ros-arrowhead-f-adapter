# Implementation documentation for ros_publisher

## SoSD
''ros_publisher'' is a system that contains a publisher and subscriber.
The publisher is a Robotic Operating System (ROS) node.
The subscriber is a Arrowhead framework node that only print the received data.
These system is constructed ass a prof of concept and skeleton for arrowhead connecting to ROS ass a legacy devise.
Dud to the main purpose of these system will these document focus on the publisher and not the subscriber.


## SoSDD
To achieve the ROS-Arrowhead framework interaction is the [client-cpp](https://github.com/arrowhead-f/client-cpp) code used ass a base.
The code are how ever re worked some to work ass a publisher subscriber couple.
The old provider is reworked into the ROS subscriber.
In `ros_publisher.cpp` is the main function.
It all sow contains the access point to ROS and Arrowhead.
In SubscriberExample is the pure Arrowhead subscriber placed.
It has some funky naming but the plan is to rewrite most of it (don't fell for double work).
The messages that are sent between are of json format.

Internally is Arrowhead core system used by the publisher and subscriber access orchestration, authorisation and servisRegistry.

##SysD
The publisher has the system name ''publisher_ros_insecure'' and is publishing the service ''example_temperature''.
The subscriber is named ''subscrier_ah_example_insecure''.
The protocol that are used to communicate between these are json SenML.
The messages has the structure:
```JSON
{
 	"Entity":[{
 		"ID": "this_is_the_sensor_id",
 		"Temperature":$temperature,
 		"Time_stamp": "$time_stamp"}],
	"BaseName": "$serviceID",
 	"Unit": "Celsius"
}
```
In the publisher is all sow a ROS publisher that are use in debugging purpose.
It publish on topic `/temperature_example` and the messages are of type `sensor_msgs/Temperature`


##SysDD
Below follows a walk thru of the subscriber code (arrowhead interface code are excluded because it is supposed to work ass a library).

### service.cpp
The main file [ros_subscriber.cpp](src/ros_subscriber.cpp).

```cpp
Converter convert;
SensorHandler oSensorHandler;
```
Creates a instances of the classes `Converter` and `SensorHandler`.
`Converter` is used to convert messages between ROS standard and Arrowhead json standard.
`SensorHandler` is the interface access point to Arrowhead.


```cpp
	ros::init(argc, argv, "subscriber_example");
	ros::NodeHandle n;
	ros::Publisher rescived_pub = n.advertise<sensor_msgs::Temperature>("temperature_example", 10);
```
Here are the ROS nod initialised.
`inti` and `NodeHandle` are used to connect to `roscore`.
`Publisher` sets these node as a publisher on the topic ''temperature_example'' whit the messages of type ''sensor_msgs::Temperature''.
You can all sow see that a buffer of size ''10'' is used.


```cpp
	ros::NodeHandle nh("~");
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", oSensorHandler.config.SECURE_PROVIDER_INTERFACE , false);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", oSensorHandler.config.SECURE_ARROWHEAD_INTERFACE , false);
	nh.param<std::string>("ACCESS_URI", oSensorHandler.config.ACCESS_URI ,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<std::string>("ACCESS_URI_HTTPS", oSensorHandler.config.ACCESS_URI_HTTPS ,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	nh.param<int>("THIS_PORT", oSensorHandler.config.THIS_PORT, 8454);
	nh.param<std::string>("THIS_ADDRESS", oSensorHandler.config.THIS_ADDRESS, "10.0.0.40");
	nh.param<std::string>("THIS_ADDRESS6", oSensorHandler.config.THIS_ADDRESS6, "[fe80::1a57:58d9:c43:6319]");
	nh.param<std::string>("THIS_SYSTEM_NAME", oSensorHandler.config.THIS_SYSTEM_NAME, "client1");
	nh.param<std::string>("AUTHENTICATION_INFO", oSensorHandler.config.AUTHENTICATION_INFO, "null");
	nh.param<std::string>("SERVICE_DEFINITION", oSensorHandler.config.SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("INTERFACE", oSensorHandler.config.INTERFACE, "REST-JSON-SENML");
	nh.param<std::string>("SECURITY", oSensorHandler.config.SECURITY, "toke");
	nh.param<bool>("OVERRIDE_STORE", oSensorHandler.config.OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", oSensorHandler.config.MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", oSensorHandler.config.METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", oSensorHandler.config.PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", oSensorHandler.config.ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", oSensorHandler.config.EXTERNAL_SERVICE_REQUEST, false);
	nh.param<std::string>("TARGET_SYSTEM_NAME", oSensorHandler.config.TARGET_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("TARGET_ADDRESS", oSensorHandler.config.TARGET_ADDRESS, "10.10.10.122");
	nh.param<int>("TARGET_PORT", oSensorHandler.config.TARGET_PORT, 8452);
	nh.param<std::string>("UNIT", oSensorHandler.config.UNIT, "NOT SPIFFED");	
```
Here is the parameters form the launch file red in to the program.
`oSensorHandler.config` is a struct containing the configure variables.
If there is a need to add settings is the struct defined in ''[arrowhead_data_ext.hpp](src/Consumer/arrowhead_data_ext.hpp)''
The structure of the ''nh.param'' is:
`nh.param<variabel_type>("name_in_lanch_file", variabel_name, default_value_if_no_input)`.

```cpp
	convert.init("sensor_id", oSensorHandler.config.UNIT, oSensorHandler.config.THIS_SYSTEM_NAME);
```
Now when the configure variables are set can the Arrowhead system be properly initialised.
`init` sets some constant in the  messages.

```cpp
	convert.set(404, 1);
```
These lines should not be dun in these way.
It is sets the messages to a fake value.
They exist to enable fast and easy testing.
But be aware of the message is initialised is a race condition likely.

```cpp
	ros::Rate loop_rate(0.3);
	int i = 0;
	while (ros::ok()) {
		convert.set(i++, std::time(0));
    	oSensorHandler.processConsumer(convert.getJsonMsgs());
		rescived_pub.publish(convert.temperature);
		ros::spinOnce();
		loop_rate.sleep();
	}
```
First is the loop rate set and a counter acting temperature sensor set.  
1 The messages is updated.   
2 The messages is published on the Arrowhead system.  
3 A message is published on the ROS system to prov it works on both.  
4 Wait for interrupt and pass before next lop lap.  

Noticeable is that the loop_rate is set in Hz and these whey to do it is very inaccurate.

### messages.cpp
The messages file are used to convert the messages data between different formats.


```cpp
	obj = json_object_new_object();	
	json_object *arr_obj = json_object_new_array();
	json_object *arr_cont = json_object_new_object();
```
Creates the needed `json_objects`.

```cpp
	std::string str(ptr);
	struct json_object *obj;
	obj = json_tokener_parse(str.c_str()); //extract a json object
	struct json_object *entity;
	json_object_object_get_ex(obj, "Entity", &entity);
```
These lines sets the incoming messages to be treated ass a `json_object` and extract the array containing the interesting data.


```cpp
	struct json_object *time;
	json_object_object_get_ex(
		json_object_array_get_idx(entity,0), "Time_stamp", &time);
	int TIME = json_object_get_int(time);
	if (TIME > temperature.header.stamp.sec){
```
These lines extract the time stamp and check if the messages is a newer messages than the last.
These is security agents delayed packages.

```cpp
		temperature.header.stamp.sec  = TIME;
		struct json_object *temp;
		json_object_object_get_ex(
			json_object_array_get_idx(entity,0), "Temperature", &temp);
		temperature.temperature  = json_object_get_double(temp);	
```
In side the if is the messages stored to the ros messages `sensor_msgs/Temperature` called temperature.


```cpp
	json_object_object_add(arr_cont,"ID", json_object_new_string(sensor_id.c_str()));
	json_object_object_add(arr_cont,"Temperature", json_object_new_double(temp));
	json_object_object_add(arr_cont,"Time_stamp", json_object_new_int(time));
	json_object_array_add(arr_obj, arr_cont);
```
The first part of the message is a array of size 1.
The internal element is a `json_object`.

##IDD
To implement your one ROS Publisher can you copy these project and remove the example subscriber.
Then can you change the values in [launch/ros_publisher.launch](launch/ros_publisher.launch) to mach your system.
You can all sow change the projects name, but remember to change it in [ros_publisher.launch](launnch/ros_publicher.launch), in [CMakeLists.txt](CMakeLists.txt) and in [package.xml](package.xml) ass well ass the name of the folder and launch file.

Then is it necessary (probably) to change [messages.cpp] (src/messages.cpp) sow it fit your resource.
Finally is it only to extend [ros_publisher.cpp](src/publicher.cpp) as a normal ROS node ([learn how](http://wiki.ros.org/ROS/Tutorials)).
Hopefully is there no need for you to make changes in other places.


### Parameters


| Parameter		 				| Type 			| Usages |
|:------------------------------|:--------------|:----------------------------|
| SERVICE_DEFINITION			| std::string	| service name|
| INTERFACE						| std::string	| The interface used for the messages |
| CUSTOM_URL					| std::string	| A url used to find the correct provider|
| UNIT							| std::string	| The unit of the data|
| SECURITY						| std::string	| Security protocol used, typical ''token''|
| ACCESS_URI					| std::string	| Url to core systems |
| ACCESS_URI_HTTPS				| std::string	| Url to core systems https|
| OVERRIDE_STORE				| std::string	| Orchestrator interface setting|
| MATCHMAKING					| std::string	| Orchestrator interface setting|
| PING_PROVIDER					| std::string	| Orchestrator interface setting|
| ONLY_PREFERRED				| std::string	| Orchestrator interface setting|
| EXTERNAL_SERVICE_REQUEST		| std::string	| Orchestrator interface setting|
| THIS_SYSTEM_NAME				| std::string	| The this prosumer system name|
| THIS_ADDRESS					| std::string	| The ipv4 address to the this prosumer|
| THIS_ADDRESS6					| std::string	| The ipv6 address to the this prosumer|
| THIS_PORT						| int			| The port the this prosumer uses|
| TARGET_SYSTEM_NAME			| std::string	| The system name to the target procumer|
| TARGET_ADDRESS				| std::string	| The ipv4 address to the target prosumer|
| TARGET_PORT					| int 			| The port that the target prosumer uses |
| SECURE_ARROWHEAD_INTERFACE	| bool			| Set if the arrowhead interface should be secure|
| SECURE_PROVIDER_INTERFACE		| bool			| Set if the provider interface should be secure|
| PUBLIC_KEY_PATH				| std::string	| A absolute path to a public key|
| PRIVATE_KEY_PATH				| std::string	| A absolute path to a matching private key|
| AUTHENTICATION_INFO			| std::string	| Public RSA key, set by code from PUBLIC_KEY_PATH|
