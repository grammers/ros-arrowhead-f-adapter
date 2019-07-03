# Implementation documentation for service_consumer
## SoSD
''service_consumer'' is a system that contains one arrowhead framework consumer and one Robotic Operating System (ROS) service (ROS service is equivalent to arrowhead provider).
The ROS service is a legacy devise providing a temperature measurement (only a dummy is implemented).
These system is developed to be a prof of concept and a skeleton for interfacing arrowhead and ROS together.
It allow serves the purpose of being a demo.
Sow the main focus are on the provider/service (hens fort will provider be the term used).
The consumer do only exist for testing and demonstration sow it will not be explained her.


## SoSDD
To achieve the ROS-Arrowhead framework interaction it the [client-cpp]('https://github.com/arrowhead-f/client-cpp') code used ass a base.
The 'ProviderExample.cpp' was rewritten into 'service.cpp'.
To achieve a easy to use interface where some of the other files modified.
The functionalety are the some but the code coevality is improved.
Interactions are now dun by a few lines of cod and they are easily changed to a different context.
A '.launch' was added to simplify configuration and the returning massages is held to 'json_object' format ass ling ass possible.


At startup is a http server set up to receive request to.
The provided sensor data is registered at the serviceRegistrator.


## SysD
The provider have the system name is ''service_ros_example_insecure''.
It provide the service ''example_temperature''.
The interface is ''JSON'' and the message have the structure:
```JSON
{
 	"Entity":[{
 		"ID": "this_is_the_sensor_id",
 		"Temperature":$temperatre,
 		"Time_stamp": "$time_stamp"}],
 	"BaseName": "$baseName",
	"Unit": "Celsius"
}
```
It also subscribe on the ROS topic ''temperature_example'' with the messages sensor_msgs::Temperature.
The consumer have the system name ''consumer_ah_insecure''.

##SysDD
Below follows a wack thru of the provider code (arrowhead interface code are excluded because it is supposed to work ass a library).

### service.cpp
The main file [service.cpp]('https://github.com/grammers/ros-arrowhead-f-adapter/blob/server-consumer/service_consumer/src/service.cpp').

```cpp
Converter convert;
SensorHandler oSensorHandler;
```
Creates a instances of the classes `Converter` and `SensorHandler`.
`Converter` is used to convert messages between ROS standard and Arrowhead json standard.
`SensorHandler` is the interface access point to Arrowhead.

```cpp
void getTempCallback(const sensor_msgs::Temperature::ConstPtr& msg){
	convert.set(msg->temperature, msg->header.seq); 
    oSensorHandler.processProvider(convert.getJsonMsgs());
	return;
}
```
The callback function for the ROS subscriber.
It has the new values ass input and send throws values to `convert.set`.
`convert.set` convert the ROS message to json format.
`oSensorHandler.processProvider` is setting  the new message ass the response messages to a request from a consumer.


```cpp
	ros::init(argc, argv, "service_example");
	ros::NodeHandle n;
	ros::Subscriber temperature_sub = n.subscribe<sensor_msgs::Temperature>("temperature_example", 10,  getTempCallback);
```
Here are the ROS nod initialised.
`inti` and `NodeHandle` are used to connect to `roscore`.
`Subscriber` sets these node as a subscriber on the topic ''temperature_example'' whit the messages of type ''sensor_msgs::Temperature''.
You can all sow see that a buffer of size ''10'' is used and that the callback function is ''getTempCallback''.


```cpp
	ros::NodeHandle nh("~");
	nh.param<std::string>("SR_BASE_URI", oSensorHandler.config.SR_BASE_URI, "http://arrowhead.tmit.bme.hu:8442/serviceregistry/");
	nh.param<std::string>("SR_BASE_URI_HTTPS", oSensorHandler.config.SR_BASE_URI_HTTPS, "https://arrowhead.tmit.bme.hu:8443/serviceregistry/");
	nh.param<std::string>("PROVIDER_ADDRESS", oSensorHandler.config.PROVIDER_ADDRESS, "10.0.0.40");
	nh.param<std::string>("PROVIDER_ADDRESS6", oSensorHandler.config.PROVIDER_ADDRESS6, "[fe80::1a57:58d9:c43e:6319]");
	nh.param<std::string>("CUSTOM_URL", oSensorHandler.config.CUSTOM_URL, "this_is_the_custom_url");
	nh.param<std::string>("PROVIDER_SYSTEM_NAME", oSensorHandler.config.PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("SERVICE_DEFINITION", oSensorHandler.config.SERVICE_DEFINITION, "IndoorTemperature_ProviderExample");
	nh.param<std::string>("INTERFACE", oSensorHandler.config.INTERFACE, "JSON");
	nh.param<std::string>("PRIVATE_KEY_PATH", oSensorHandler.config.PRIVATE_KEY_PATH, "keys/tempsensor.testcloud1.private.key");
	nh.param<std::string>("PUBLIC_KEY_PATH", oSensorHandler.config.PUBLIC_KEY_PATH, "keys/tempsensor.testcloud1.publickey.pem");
	nh.param<std::string>("UNIT", oSensorHandler.config.UNIT, "Celsius");
	nh.param<std::string>("SECURITY", oSensorHandler.config.SECURITY, "token");
	nh.param<int>("PROVIDER_PORT", oSensorHandler.config.PROVIDER_PORT, 8452);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", oSensorHandler.config.SECURE_ARROWHEAD_INTERFACE, false);
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", oSensorHandler.config.SECURE_PROVIDER_INTERFACE, false);
```
Here is the parameters form the launch file red in to the program.
`oSensorHandler.config` is a struct containing the configure variables.
If there is a need to add settings is the struct defined in ''[ApplicationServiceInterface.hpp]('https://github.com/grammers/ros-arrowhead-f-adapter/blob/server-consumer/service_consumer/src/Interface/ApplicationServiceInterface.cpp')''
The structure of the ''nh.param'' is:
`nh.param<variabel_type>("name_in_lanch_file", variabel_name, default_value_if_no_input)`.

```cpp
	oSensorHandler.initSensorHandler(oSensorHandler.config.PROVIDER_SYSTEM_NAME);
	convert.init("sensor_id", oSensorHandler.config.UNIT, oSensorHandler.config.PROVIDER_SYSTEM_NAME);
```
Now when the configure variabels are set can the Arrowhead system be proporly initilised.
`initSensorHandler` set up the Arrowhead system.
`init` sets some constant in the  messages.

```cpp
	convert.set(404, 1);
    oSensorHandler.processProvider(convert.getJsonMsgs());
```
These lines should not be dun in these way.
It is sets the messages to a fake value.
They exist to enable fast and easy testing.

```cpp
	while (ros::ok()) {
		ros::spin();
	}
```
Wait for a interrupt.


### messages.cpp
The part of messages that are used in the provider is the `set` function and `getJsonMsgs` function.
And the get simply returns the constructed messages in `json_object` format.
Sow the only function to explained are the `set` function.
That function simply construct the messages that are shown above.

```cpp
	obj = json_object_new_object();	
	json_object *arr_obj = json_object_new_array();
	json_object *arr_cont = json_object_new_object();
```
Creates the needed `json_objects`.

```cpp
	json_object_object_add(arr_cont,"ID", json_object_new_string(sensor_id.c_str()));
	json_object_object_add(arr_cont,"Temperature", json_object_new_double(temp));
	json_object_object_add(arr_cont,"Time_stamp", json_object_new_int(time));
	json_object_array_add(arr_obj, arr_cont);
```
The first part of the message is a array of size 1.
The internal element is a `json_object`.
That object (arr_cont) is build.
And the array is created.

```cppp
	json_object_object_add(obj,"Entity", arr_obj);
	json_object_object_add(obj,"BaseName", json_object_new_string(identety.c_str()));
	json_object_object_add(obj,"Unit", json_object_new_string(data_unit.c_str()));
```
Finely is the array putt to gather with the rest of the objects and `obj` (the message) is build.


##IDD
To implement your one ROS provider can you copy these project and remove the example provided.
Then can you change the values in [launch/service_consumer.launch](launch/service_consumer.launch) to mach your system.
You can all sow change the projects name, but remember to change it in [service_coliet.launch](launnch/service_consumer.launch), in [CMakeLists.txt](CMakeLists.txt) and in [package.xml](package.xml) ass well ass the name of the folder and launch file.

Then is it necessary (probably) to change [messages.cpp] (src/messages.cpp) sow it fit your resource.
Finally is it only to extend [service.cpp](src/service.cpp) as a normal ROS node ([learn how](http://wiki.ros.org/ROS/Tutorials)).
Hopefully is there no need for you to make changes in other places.


### Parameters


| Parameter		 				| Type 			| Usages |
|:------------------------------|:--------------|:----------------------------|
| PUBLIC_KEY_PATH				| std::string	| A absolute path to a public key|
| PRIVATE_KEY_PATH				| std::string	| A absolute path to a matching private key|
| PROVIDER_SYSTEM_NAME			| std::string	| The providers system name|
| PROVDER_ADDRESS				| std::string	| the ipv4 address to the provider|
| PROVIDER_ADDRESS6				| std::string	| The ipv6 address to the provider|
| PROVIDER_PORT					| int			| The port the provider uses|
| SERVICE_DEFINITION			| std::string	| service name|
| CUSTOM_URL					| std::string	| A url used to find the correct provider|
| INTERFACE						| std::string	| The interface used for the messages |
| SR_BASE_URI					| std::string	| Url to core systems service registry|
| SR_BASE_URI_HTTPS				| std::string	| Url to core systems https service registry|
| UNIT							| std::string	| The unit of the data|
| SECURITY						| std::string	| Security protocol used, typical ''token''|
| AUTHENTICATION_INFO			| std::string	| Public RSA key, set by code from PUBLIC_KEY_PATH|
| SECURE_ARROWHEAD_INTERFACE	| bool			| Set if the arrowhead interface should be secure|
| SECURE_PROVIDER_INTERFACE		| bool			| Set if the provider interface should be secure|
