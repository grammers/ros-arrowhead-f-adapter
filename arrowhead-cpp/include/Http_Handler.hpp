#pragma once
#include <string>

#ifdef __linux__

#include "curl.h"
#include "microhttpd.h"

#elif _WIN32

extern "C" {
#include "include\curl\curl.h"
#include "include\mhttpd\microhttpd.h"
}
#endif

using namespace std;

class Http_Handler
{
private:
	struct MHD_Daemon *pmhd = NULL;

public:
	int SendRequest(string pdata, string paddr, string pmethod);

	virtual size_t callback_GET_http(const char *ptr, size_t size);
	virtual int httpGETCallback(const char *Id, string *pData_str);
	virtual int httpPOSTCallback(const char *url, const char *payload);

	virtual size_t httpResponseCallback(char *ptr, size_t size);

	int MakeServer(unsigned short listen_port);
	int KillServer();
};
