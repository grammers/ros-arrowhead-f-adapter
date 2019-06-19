# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(provider_client)

file(GLOB_RECURSE *.json)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	std_msgs
	message_generation
)

## Declare ROS messages and services
#add_message_files(FILES Num.msg)
add_service_files(FILES clientExample.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

configure_file (
	src/Consumer/ConsumedService.h
	src/Consumer/SensorHandler.h
	#src/temperature.h
)


## Build talker and listener
include_directories(
	${catkin_INCLUDE_DIRS}
)


add_executable(client 
	src/client.cpp
	src/Consumer/ConsumedService.cpp
	src/Consumer/SensorHandler.cpp
	src/Interface/Http_Handler.cpp
	src/Interface/Https_Handler.cpp
	src/Interface/OrchestratorInterface.cpp
	src/Interface/ini/dictionary.c
	src/Interface/ini/iniparser.c
	src/Security/base64.cpp
	src/Security/RSASecurity.cpp
	consumedServices.json
)
set_source_files_properties(src/Interface/ini/iniparser.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(src/Interface/ini/dictionary.c PROPERTIES LANGUAGE CXX)

target_link_libraries(client 
	${catkin_LIBRARIES}
	curl
	microhttpd
	crypto
	pthread
	json-c
)


# %EndTag(FULLTEXT)%
