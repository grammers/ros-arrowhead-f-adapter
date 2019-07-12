# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(consumer_publisher)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	sensor_msgs
	message_generation
	rostest
)

## Declare ROS messages and services
#add_message_files(FILES Num.msg)
#add_service_files(FILES Example.srv)

## Generate added messages and services
#	generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()


set(PROJECT_LINK_LIBS libarrowhead_cpp.so)
link_directories(/usr/local/lib/)

#configure_file (
#)


## Build talker and listener
include_directories(
	${catkin_INCLUDE_DIRS}
)


add_executable(consumer_publisher 
	src/consumer_publisher.cpp
	../comon/messages.cpp
)

target_link_libraries(consumer_publisher 
	${catkin_LIBRARIES}
	${PROJECT_LINK_LIBS}
	json-c
)

# %EndTag(FULLTEXT)%
