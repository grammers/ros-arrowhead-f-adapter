#include <gtest/gtest.h> 
#include <ros/ros.h>

#include "../src/messages.hpp"

TEST(const_global, testcase1){
	#include "../src/const_global.hpp"
}

TEST(TestSuite, testcaserr2){
#include "../src/messages.hpp"
}

/*
TEST(TestSuite, testcaser3){
	#include "../src/Consumer/SensorHandler.h"
}
*/
TEST(TestSuite, testcase4){
	#include "../src/Consumer/ConsumedService.h"
}

/*
TEST(hFileTest, http){
	#include "../src/client.cpp"
	#include "../src/messages.hpp"
	#include "../src/Consumer/ConsumedService.h"
	#include "../src/const_global.hpp"
	#include "../src/messages.hpp"
	#include "../src/Consumer/SensorHandler.h"
	#include "../src/Interface/OrchestratorInterface.hpp"
	#include "../src/Interface/Http_Handler.hpp"
}
*/
int main(int argc, char** argv){
	ros::init(argc, argv, "tester");
	ros::NodeHandle nh;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
