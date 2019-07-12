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

namespace arrowhead {

class HttpHandler
{
private:
	struct MHD_Daemon *pmhd = NULL;

public:
	int sendRequest(std::string pdata, std::string paddr, std::string pmethod);

	virtual size_t callbackGETHttp(char *ptr, size_t size);
	virtual int httpGETCallback(const char *Id, std::string *data_str);
	virtual int httpPOSTCallback(const char *url, const char *payload);

	virtual size_t httpResponseCallback(char *ptr, size_t size);

	int MakeServer(unsigned short listen_port);
	int KillServer();
};
}
