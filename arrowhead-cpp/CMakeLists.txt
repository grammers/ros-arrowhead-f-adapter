# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(arrowhead_cpp)


#set(CMAKE_BUILD_TYPE Release)

## Build talker and listener
#file (GLOB header include/*.h)
#include_directories(${CMAKE_SOURCE_DIR})


add_library(arrowhead_cpp SHARED 
	src/arrowhead_cpp.cpp
	src/Interface/Http_Handler.cpp
	src/Interface/Https_Handler.cpp
	src/Interface/ApplicationServiceInterface.cpp
	src/Interface/OrchestratorInterface.cpp
	src/Interface/ini/dictionary.c
	src/Interface/ini/iniparser.c
	src/Security/base64.cpp
	src/Security/RSASecurity.cpp
)
set_source_files_properties(src/Interface/ini/iniparser.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(src/Interface/ini/dictionary.c PROPERTIES LANGUAGE CXX)

target_link_libraries(arrowhead_cpp 
	curl
	microhttpd
	crypto
	pthread
	json-c
	gnutls
)

# tests
#catkin_add_gtest(provider_client-test test/htest.cpp)
#if(TARGET provider_client-test)
#		target_link_libraries(provider_client-test ${catkin_LIBRARIES})
#endif()

set(dirs "include")
include_directories("${dirs}")

install(TARGETS arrowhead_cpp DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/arrowhead)


# %EndTag(FULLTEXT)%
