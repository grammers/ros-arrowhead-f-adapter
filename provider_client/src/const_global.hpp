#pragma once
#include <iostream>

// global variables for system set up
// DO NOT change these in runtime.
// they are supposed to be constant but ROS pram intake function do not allow it.

// security
extern bool SECURE_PROVIDER_INTERFACE;
extern bool SECURE_ARROWHEAD_INTERFACE;
// Orchestrator
extern bool OVERRIDE_STORE;
extern bool MATCHMAKING;
extern bool METADATA_SEARCH;
extern bool PING_PROVIDERS;
extern bool ONLY_PREFERRED;
extern bool EXTERNAL_SERVICE_REQUEST;

// client/consumer 
extern std::string OR_BASE_URI;
extern std::string OR_BASE_URI_HTTPS;
extern std::string CLIENT_ADDRESS;
extern std::string CLIENT_ADDRESS6;
extern std::string CONSUMER_ID;
extern std::string CLIENT_SYSTEM_NAME;
extern std::string AUTHENTICATION_INFO;
// requested service
extern std::string SERVICE_DEFINITION;
extern std::string INTERFACES;
extern std::string SECURITY;
extern std::string PROVIDER_SYSTEM_NAME;
extern std::string PROVIDER_ADDRESS;

// ports
extern int CLIENT_PORT;
extern int PROVIDER_PORT;
