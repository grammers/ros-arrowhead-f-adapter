# Arrowhead framework provider to ROS client
These is an adapter for arrowhead framework to enable the use of Robotic Operating System (ROS) ass a legacy devise.
These specific repository is for a Arrowhead provider with a ROS client (client is ROS's version of arrowheads consumer).

The code contains a stupid example that can be used ass a skeleton to build your one arrowhead/ROS system.
It is currently in development sow all features are not in place yet (see TODO in order to see what is in the pipeline).
Currently is it only the insecure interfaces that are working.

## Install
Install [Arrowhead framworke](https://github.com/arrowhead-f/core-java).  
And install [ROS](http://wiki.ros.org/ROS/Installation) in development is Meloic Morenia used.

## build
You have to the Arrowhead provider and ROS client separately.

### build Arrowhead provider
Navigate to the ProviderExampel folder and run
```
make
```
to build the Arrowhead provider.

### Build ROS client
Navigate to some where in the catkin work space and run
```
catkin build provider_client
```
to build the ROS client.  

## RUN
To run the example is it necessary you have a mysql data base correct set up.
It might is necessary to name it "arrowhead" and have pass word set to "arrowhead" (have not tested these).
If you choose different settings you might need to adjust settings some where in the code.

There three places where settings are set these settings need to be in agreement with etch other.
The relevant files are:  
[launch/provider_client.launch](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/provider_client/launch/provider_client.launch) for the client settings  
[ProviderExample/ApplicationServiceInterface.ini](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/provider_client/ProviderExample/ApplicationServiceInterface.ini) for provider settings   
[ProviderExample/src/Provider/ProvidedService.h](https://github.com/grammers/ros-arrowhead-f-adapter/blob/master/provider_client/ProviderExample/src/Provider/ProvidedService.h) for additional provider settings.  

You will have to change the ip addresses to your ip.
Check your ip by running ```ifconfig```.
You might have to change ports to something your system allow.

In the data base will you have to add the client to "arrowhead_system" and the connection in "intra_cloud_authorization"
To do that run:
```
mysql -u arrowhead -p
use arrowhead;
insert into arrowhead_system (id, address, port, system_name) value ([unused id], [your ip], [port you gonna use], "client_ros_insecure");
insert into intra_cloud_authorization (id, consumer_system_id, provider_system_id, arrowhead_service_id) value ([unused id], [the id you set for client], [your providers id], [the service id]);
exit;
```

To find the values needed can the following command be usefully to run while you are in the data base.
To see available id and ports (all occupied ports are not listed her) and also see your providers id (default system_name for the provider is "InsecureTemperatureSensor).
```
select * from arrowhead_system;
```
To see your service id (default service_definition is IndoorTemperature_ProviderExample)
```
select * from arrowhead_service;
```

To finally run the provider navigate to ProviderExampel folder and run:
```
./run.sh
```
and to run the client open a new terminal and run:
```
roslaunch provider_client provider_client.launch
```

## Debugging and viewing
To debug and following what’s happening can the following be to help.

View prints form ros node
```
cat ~/.ros/log/latest/client-2-stdout.log
```

To see the published ROS topic
```
rostopic echo /client_demo
```



## TODO
implemante json formating for internal messgesin in setup
comment  
create demo guide  
fix HTTP  
code cleanup  
