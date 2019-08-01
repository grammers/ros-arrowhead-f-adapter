# ROS-arrowhead-f-adapter
A adapter to run ROS as a legacy node in arrowhead framework.
These is an example for how you can run ROS ass a legacy devise for
Arrowhead framework.
Three examples haw been implemented.
These examples are created sow they can be run together ass a big
example.
Together do they cover all four service types: provider, consumer,
publisher and subscriber.

The development and testing have been dune on ubuntu 18.04 and ROS melodic.
The code is written in c++.
There fore is a c++ client library for arrowhead needed.


## Pre-requests
[ROS](http://wiki.ros.org/ROS/Installation)  
[Arrowhead core](https://github.com/arrowhead-f/core-java)  
[client-cpp](https://github.com/grammers/client-cpp)

## The examples
The examples are sending a temperature from provider to subscriber thru
consumer_publisher.
In consumer_publisher is the temperature converted from Celsius to
Fahrenheit.
In all steps are a ROS massages published (`sensor_msgs/Temperature`) containing the temperature and a time stamp.
Between the different nodes are the messages sent using Arrowhead framework.
The examples name describe what type of Arrowhead node it is.

### Run
First do you have to have arrowhead core systems running.
Depending on your system and installation, may you start them in different
ways.
One way is to `cd` to the core system `{root folder}/scripts` and run a start up
script.
You also need to configure the data base with the system configuration
you uses.
For the examples.

> mysql -u arrowhead -p  
> arrowhead #the password   
> use arrowhead;  
> select \* from arrowhead_systems;

Select a not used `id` (ex. 1).

> insert into arrowhead_system (id, address, port, system_name) value (1, ''130.240.5.68'', 8492, ''temperature_consumer_demo'');

> select \* from arrowhead_service;  
> select \* from intra_cloud_authorization;

Note the `id` of ''temperature_consumer_demo'', ''temperature_provider_demo'', ''temperature_subscriber_demo'', ''temperature_c'', and ''temperature_f''.
If they aren't there, try to run the system once.
Presume that the `id` are (in the some order) 1, 2, 3, 4 and 5.
Check for free `id` in intra_cloud_authorization, Presume 6 and 7 are free.
Then run

> insert into intra_cloud_authorization value(6, 1, 2, 4);  
> insert into intra_cloud_authorization value(7, 1, 3, 5);

and to exit.

> exit;


The nodes can be run one by one using.

> roslaunch provider provider.launch  
> roslaunch consumer_provider consumer_provider.launch  
> roslaunch subscriber subscriber.launch  

Or they can be run as one.  

> roslaunch provider system.launch  

Launch parameters are set in the file you actually lunches.

#### Outprint
To see the propagation thru the system.

> rostopic echo temperature_provider  
> rostopic echo temperature_consumer  
> rostopic echo temperature_publisher  
> rostopic echo temperature_subscriber

#### RUN all
To run all nods and open a `tmux` window with the four topics echoing, run
`./deploy_system.sh`.
To install `tmux` run `sudo apt install tmux`.
The appearing window will have fore columns.
Etch column will echo one topic.
The echoed topics are, from left, `temperature_provider`,
`temperature_consumer`, `temperature_publisher` and
`temperature_subscriber`.


## Installation
Preformed on ubuntu 18.04 server but should work on most systems.
ROS is probably the limiting factor for installing in different os.

### Arrowhead framework core
There are some prerequisites. 

> sudo apt install openssl libgnutls28-dev libgnutlsxx28 libssl1.1 libssl1.0-dev libcurl3 libcurl3-gnutls libcurl4-gnutls-dev libcrypto++-dev libcrypto++-utils libcrypto++6 libgpg-error-dev automake texinfo g++ libjson-c-dev mysql-server maven openjdk-11-jdk-headless make cmake   

You will also need `microhttod`.

> wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-latest.tar.gz  
> tar -xvzf libmicrohttpd-latest.tar.gz  
> cd libmicrohttpd-0.9.65  

Or what the latest version is when you install.

> ./configure --with-gnutls  
> sudo make install  

And ad it to your build path.

> cd /usr/lib  
> sudo ln -s /usr/local/lib/libmicrohttpd.so.12.54.0 libmicrohttpd.so.12  

Now is it time to configure the data base.

> sudo /etc/init.d/mysql stop  
> sudo mkdir -p /var/run/mysqld && sudo chown mysql /var/run/mysqld  

Start mysql    
> sudo mysqld_safe --skip-grant-tables &  
```
mysql -u root
use mysql;
update user set authentication_string=PASSWORD("root") where User='root';
update user set plugin="mysql_native_password" where User='root';
flush privileges;
quit;
```
> sudo killall mysqld  
> sudo /etc/init.d/mysql start  

Other password and user name can be used, but you will have to modify
arrowhead core there after.
Decide where you want arrowhead core to be stored.
For example: `mkdir arrowhead`, `cd arrrowhead`.

> git clone https://github.com/arrowhead-f/core-java.git  
> cd core-java  
> mvn package  
> mvn install  
> sudo dpkq -i target/\*.deb 

You will reserve a prompt.
Use the following settings.

```
Detached
user arrowhead
pw arrowhead
mysql pw root # you set erlier
```
And start arrowhead core.

> cd scripts  
> sudo chmod +x start_insecure_coresystem.sh  
> ./start_insecure_coresystem.sh

Time to install the cpp library.

> cd ..  
> git clone https://github.com/grammers/client-cpp.git  
> cd client-cpp  
> cmake CMakeLists.txt  
> sudo make install  

For installation of ROS follow [these guide](http://wiki.ros.org/ROS/Installation).
And for some additional tools and setup follow instructions below.

> sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" > /etc/apt/sources.list.d/ros-latest.list'  
> wget http://packages.ros.org/ros.key -O - | sudo apt-key add -  
> sudo apt update  
> sudo apt install python-catkin-tool  

And setup the workspace.

> source /opt/ros/melodic/setup.bash  
> mkdir -p ~/catkin_ws/src  
> cd ~/catkin_ws  
> catkin init  
> source devel/setup.bash

To simplify future use you might want to add `source
~/catkin_ws/devel/setup.bash` to your `.bashrc`.

> echo "source ~/catkin_ws/devel/setup.bash" >> .bashrc

If you want to learn more about ROS can you continue the tutorial you started
in last step.
Observe you are to use `catkin build` instead of `catkin make`.

> cd catkin_ws/src  
> git clone https://github.com/grammers/ros-arrowhead-f-adapter.git  
> catkin build


## Usages
The connection between ROS and Arrowhead framework are weak.
Sow it is no problem to develop the ROS system and Arrowhead framework
system separately and only include both where they interact.
All that is needed is initialise bout ass you do normally and writ the code
you desire for the connection.

Implementation in other languages should be as easy (these is not tested).
ROS also supports python sow all that is needed is a python arrowhead
framework library.
