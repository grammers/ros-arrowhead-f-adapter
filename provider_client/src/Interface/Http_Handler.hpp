#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <list>
//#ifdef __linux__
#include "ini/iniparser.h"
extern "C" {
#include "microhttpd.h"
#include "include/curl/curl.h"
}
//#elif _WIN32

//extern "C" {
//#include "include\curl\curl.h"
//#include "include\mhttpd\microhttpd.h"
//}

//#endif

using namespace std;

class Http_Handler
{
private:
	struct MHD_Daemon *pmhd = NULL;

public:
	int SendRequest(string pdata, string paddr, string pmethod);

	virtual int httpGETCallback(const char *Id, string *pData_str);

	virtual size_t httpResponseCallback(char *ptr, size_t size);

	int MakeServer(unsigned short listen_port);
	int KillServer();

};
