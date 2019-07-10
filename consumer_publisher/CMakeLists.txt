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
#set_source_files_properties(src/Interface/ini/iniparser.c PROPERTIES LANGUAGE CXX)
#set_source_files_properties(src/Interface/ini/dictionary.c PROPERTIES LANGUAGE CXX)

target_link_libraries(consumer_publisher 
	${catkin_LIBRARIES}
	${PROJECT_LINK_LIBS}
	#		curl
	#	microhttpd
	#crypto
	#pthread
	json-c
	#gnutls
	
)
# tests
#catkin_add_gtest(consumer_publisher_client-test test/htest.cpp)
#if(TARGET consumer_publisher_client-test)
#		target_link_libraries(consumer_publisher_client-test ${catkin_LIBRARIES})
#endif()

# %EndTag(FULLTEXT)%
