# Arrowhead framework consumer to ROS service
These is an adapter for arrowhead framework to enable the use of Robotic Operating System (ROS) ass a legacy devise.
These specific repository is for a Arrowhead consumer  with a ROS service (service is ROS's version of arrowheads provider).

The code contains a stupid example that can be used ass a skeleton to build your one arrowhead/ROS system.
Currently is it only the insecure interfaces that are working.

## Install
Install [Arrowhead framworke](https://github.com/arrowhead-f/core-java).  
And install [ROS](http://wiki.ros.org/ROS/Installation) in development is Meloic Morenia used.

## Build
You have to the Arrowhead consumer and ROS service separately.

### Build Arrowhead consumer
Navigate to the ConsuemerExample folder and run
```
make
```
to build the Arrowhead consumer.

### Build ROS client
Navigate to some where in the catkin work space and run
```
catkin build service_consumer
```
to build the ROS service.  

## RUN
To run the example is it necessary you have a mysql data base correct set up.
It might be necessary to name it "arrowhead" and have pass word set to "arrowhead" (have not tested these).
If you choose different settings you might need to adjust settings some where in the code (of the core system).

There three places where settings are set these settings need to be in agreement with etch other.
The relevant files are:  
[launch/service_consumer.launch](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/service_consumer/launch/service_consumer.launch) for the service settings  
[ConsumerExample/OrchestratorInterface.ini](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/service_consumer/ConsumerExample/OrchestratorInterface.ini) for consumer settings   
[ConsumerExample/consumedServices.json](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/service_consumer/ConsumerExample/consumedServices.json) for additional consumer settings.  

You will have to change the ip addresses to your ip.
Check your ip by running ```ifconfig```.
You might have to change ports to something your system allow.

In the data base will you have to add the consumer to "arrowhead_system" and the connection in "intra_cloud_authorization"
To do that run:
```
mysql -u arrowhead -p
use arrowhead;
insert into arrowhead_system (id, address, port, system_name) value ([unused id], [your ip], [port you gonna use], "consumer_ah_insecure");
insert into intra_cloud_authorization (id, consumer_system_id, provider_system_id, arrowhead_service_id) value ([unused id], [the id you set for client], [your providers id], [the service id]);
exit;
```

To find the values needed can the following command be usefully to run while you are in the data base.
To see available id and ports (all occupied ports are not listed her) and also see your providers id (default system_name for the provider is ''service_ros_example_insecure'').
```
select * from arrowhead_system;
```
To see your service id (default service_definition is ''example_temperature'')
```
select * from arrowhead_service;
```

To finally run the provider navigate to ProviderExampel folder and run:
```
./run.sh
```
and to run the client open a new terminal and run:
```
roslaunch service_consumer service_consumer.launch
```

## Debugging and viewing
To debug and following what’s happening can the following be to help.

View prints form ros node
```
cat ~/.ros/log/latest/service-2-stdout.log
```

To see the subscribed ROS topic
```
rostopic echo /temperature_example
```
The topic is to set a nu readeing form a sensor.


## TODO
create demo guide  
fix HTTPs  
