# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(arrowhead_cpp)


add_library(arrowhead_cpp SHARED 
	src/Provider.cpp
	src/Subscriber.cpp
	src/Consumer.cpp
	src/Publisher.cpp
	src/Interface/HttpHandler.cpp
	src/Interface/HttpsHandler.cpp
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

set(dirs "include")
include_directories("${dirs}")

install(TARGETS arrowhead_cpp DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/arrowhead)


# %EndTag(FULLTEXT)%
