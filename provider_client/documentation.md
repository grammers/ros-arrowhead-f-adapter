# Implementation documentation for provider_client
## SoSD
''provider_client'' is a system that contains one provider and one consumer.
The consumer are a legacy devise to ROS.
The provided resource in a dummy float.
These system is developed to be a prof of concept, skeleton and demo of arrowhead connecting ROS as a legacy devise.
Thanks to the main peeress of these system is the focus on the consumer (often called client do to ROS terminology).
It is the intended meaning to replace the provider with something useful when an actual implementation are done.
There for is the documentation angel to understand how the consumer works.

![alt text](privider_client_uml.png)
UML diagram of the system.

![alt text](provider_client_usercase.png)
A sequence diagram for a request form consumer from provider.


There is currently no active security protocol working.

## SoSDD
To achieve the ROS-Arrowhead framework interaction is the [client-cpp](https://github.com/arrowhead-f/client-cpp) code used ass a base.
The ConsumerExample.cpp is rewritten to become a ROS nod.
The arrowhead connection code is kept and the rest of the file in client-cpp are functional unchanged.
A ''.launch'' file was added to simplify setting management.
The messages file are to convert between Arrowhead json-SenML and ROS messages.


Internally is the consumer utilising the orchestrator, that in its tern utilises authorization and servisRegestry, to communicate with the provider.


## SysD
The provider has the system_name InsecureTemperatureSensor and provides the services IndoorTemperature_ProviderExample.
The interface protocol is ''rest-json-SenML''.
The json return messages has the stricture: 
```
 {
 	"e":[{
 		"n": "this_is_the_sensor_id",
 		"v":$temperature,
 		"t": "$time_stamp"}],
	"bn": "$serviceID",
 	"bu": "Celsius"
}
```
The consumer has the system name ''client_ros_insecure''.
It consumer the service ''IndoorTemperature_providerExample''.
The consumer transform the incoming messages to a ROS messages of type std_msgs::Float32.
The ROS messages are published on topic ''client_demo''.

## SysDD
TODO these section shod describe the code in detail.
Below follows a walk thru of the code.


### client.cpp
In the main file [client.cpp](src/client.cpp)
```cpp
#define b2s(x) ((x)?"true":"false") 
```
A macro used to simplify and strim line printing of boolean.
It converts true to the string ''true'' and false to the string ''false''.

```cpp
bool SECURE_PROVIDER_INTERFACE;
bool SECURE_ARROWHEAD_INTERFACE;
std::string OR_BASE_URI;
std::string OR_BASE_URI_HTTPS;
int CLIENT_PORT;
std::string CLIENT_ADDRESS;
std::string CLIENT_ADDRESS6;
std::string CONSUMER_ID;
std::string CLIENT_SYSTEM_NAME;
std::string AUTHENTICATION_INFO;
std::string SERVICE_DEFINITION;
std::string INTERFACES;
std::string SECURITY;
bool OVERRIDE_STORE;
bool MATCHMAKING;
bool METADATA_SEARCH;
bool PING_PROVIDERS;
bool ONLY_PREFERRED;
bool EXTERNAL_SERVICE_REQUEST;
std::string PROVIDER_SYSTEM_NAME;
std::string PROVIDER_ADDRESS;
int PROVIDER_PORT;
```
Defines the global setup variables. These are to be treated ass constant.
The method to set them do not allow to declare them ass `const`.
These is the variables that are sett in the lunch file.


Inside ''main''.
```cpp
	ros::init(argc, argv, "client_example");
	ros::NodeHandle n;
	ros::Publisher rescived_pub = n.advertise<std_msgs::Float32>("client_demo", 10);
```
Set up for ROS.
`ros::init` and `ros::NodeHandler` are necessary for ROS nodes and the communication to ''roscore''.
A `ros::Publisher` creates a publisher node.
It advertise a std_msgs::Float32 to topic ''client_demo'' with a buffer of size 10.


```cpp
	ros::NodeHandle nh("~");
	nh.param<bool>("SECURE_PROVIDER_INTERFACE", SECURE_PROVIDER_INTERFACE , false);
	nh.param<bool>("SECURE_ARROWHEAD_INTERFACE", SECURE_ARROWHEAD_INTERFACE , false);
	nh.param<std::string>("OR_BASE_URI", OR_BASE_URI ,"http://arrowhead.tmit.bme.hu:8440/orchestrator/orchestration");
	nh.param<std::string>("OR_BASE_URI_HTTPS", OR_BASE_URI_HTTPS ,"https://arrowhead.tmit.bme.hu:8441/orchestrator/orchestration");
	nh.param<int>("CLIENT_PORT", CLIENT_PORT, 8454);
	nh.param<std::string>("CLIENT_ADDRESS", CLIENT_ADDRESS, "10.0.0.40");
	nh.param<std::string>("CLIENT_ADDRESS6", CLIENT_ADDRESS6, "[fe80::1a57:58d9:c43:6319]");
	nh.param<std::string>("CONSUMER_ID",CONSUMER_ID, "TestconsumerID");
	nh.param<std::string>("CLIENT_SYSTEM_NAME", CLIENT_SYSTEM_NAME, "client1");
	nh.param<std::string>("AUTHENTICATION_INFO", AUTHENTICATION_INFO, "null");
	nh.param<std::string>("SERVICE_DEFINITION", SERVICE_DEFINITION,"IndoorTemperature_providerExample");
	nh.param<std::string>("INTERFACES", INTERFACES, "REST-JSON-SENML");
	nh.param<std::string>("SECURITY", SECURITY, "toke");
	nh.param<bool>("OVERRIDE_STORE", OVERRIDE_STORE, true);
	nh.param<bool>("MATCHMAKING", MATCHMAKING, true);
	nh.param<bool>("METADATA_SEARCH", METADATA_SEARCH, false);
	nh.param<bool>("PING_PROVIDERS", PING_PROVIDERS, false);
	nh.param<bool>("ONLY_PREFERRED", ONLY_PREFERRED, true);
	nh.param<bool>("EXTERNAL_SERVICE_REQUEST", EXTERNAL_SERVICE_REQUEST, false);
	nh.param<std::string>("PROVIDER_SYSTEM_NAME", PROVIDER_SYSTEM_NAME, "SecureTemperatureSensor");
	nh.param<std::string>("PROVIDER_ADDRESS", PROVIDER_ADDRESS, "10.10.10.122");
	nh.param<int>("PROVIDER_PORT", PROVIDER_PORT, 8452);
```
The second `ros::NodeHandle` is weirdly needed to be able to read form the launch file (it shod be possible to do with out).
The following liens are reading from launch file and set settings of corresponding variables. 
Structure of the compand is `nh.param<[type]>("[name used in launch]", [variable name], [default value if no valu is sent]);`


```cpp
	bool debug = true;
	if(debug)
		debugPrint();
```
Enable a debug print for the launch variables.
If debug is true is the values of the global variables printed.
The prints can be seen in `~/.ros/log/latest`.

```cpp
	Converter oConverter;
	SensorHandler oSensorHandler;
```
Create instances of needed classes.
`Conveter` are the class in messages and `SensorHandler` are the entry point to the arrowhead part.


```cpp
	std::string consumerMessageExample = 
		(std::string)"{\"bn\":\"TestconsumerID\"\n}";
	ros::Rate loop_rate(0.3);
```
`consumerMessageExample` is a set up to select correct services.
It is in json - SenML format.
`loop_rate` sets the update frequency in witch a new request are sent.
It is a ruff estimate of the actual frequency.
In these example is it set to 0.3 Hz, ruffly one request every 3s.

```cpp
	while(ros::ok())
	{
```
A while loop that lops until it is terminated.
In side is all the requests and publishing don.

```cpp
   	oSensorHandler.processConsumer(consumerMessageExample);
```
Sends a arrowhead request.
It is a syncrone operation sow the system will wait for a response


```cpp
	rescived_pub.publish(oConverter.temperature);
```
Publish the reserved data.
The actual data is stored in the `OConverter` class.

```cpp
	ros::spinOnce();
	loop_rate.sleep();
```
Here is a sleep preformed until it is time for next request.
`ros::spineOnce();` is preformed to scan the ros system for any thing to react on.


### messages.cpp

```cpp
	std_msgs::Float32 Converter::temperature;
```
The relevant data in ROS standard messages type.
Declare ass static to enable setting form on please and reading form an other.

```cpp
void Converter::parce(char *ptr){
```
The function that take the reopens from the request and extract re relevant information.

```cpp
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
```
The response is of json - SenMl type.
Sow first is the message is typecast to be an actual json object.
From that object is then et elements at value ''e'' extracted.
The value ''e'' is of type array.
At the first position is a json object with the value ''v''.
Sow one lined is the ''v'' value extracted.

```cpp
	temperature.data  = json_object_get_double(v);	
```
The value ''v'' is than typecast ed to the storages variable.
The type 'std_msgs::Float32' is a ROS messages type and they exist of multiple internal variables.
These one has only one.
It is called ''data'' and in of type Float32.


## IDD
To implement your one ROS consumer can you copy these project and remove the example provider.
Then you change the values in [launch/provider_client.launch](launch/provider_client.launch) to mach your system.
You can all sowed change the projects name but remember to change it in [provider_client.launch](launch/provider_clinet.launch), in [CMakeLists.txt](CMakeList.txt) and in [package.xml](package.xml) ass well ass the name of the folder and launch file.

Then is it necessary (probably) to modify the [messages.cpp](src/messages.cpp) and [messages.hpp](messages.hpp) to match your resources messages.
How ever be careful with changing names on the fill or the function.
[SensorHandler](src/Consumer/SensorHandler.h) is including and using it.
And finally is it only to extend the ROS system as if it where a regular ROS node ([learn how](http://wiki.ros.org/ROS/Tutorials)).

If you desire to utilise a different interfaces do you have to update code in other files to.
At writhing moment do I not know how.
Sow if you know, pleas writ it down!
I haven't tested all settings in the launch file sow there mighty be some that's not easily changed.
