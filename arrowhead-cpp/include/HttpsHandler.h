#pragma once
#include <string>

#ifdef __linux__

#include "curl.h"
#include "microhttpd.h"

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#endif

namespace arrowhead {

//password protected private key
#define SERVERKEYFILE	"keys/tempsensor.testcloud1.private.key" 
#define SERVERCERTFILE	"keys/tempsensor.testcloud1.publicCert.pem"
#define ROOTCACERTFILE   "keys/tempsensor.testcloud1.caCert.pem"

class HttpsHandler
{
private:
	struct MHD_Daemon *pmhd = NULL;
	char *key_pem;
	char *cert_pem;
	char *root_ca_pem;

public:
	int SendHttpsRequest(std::string pdata, std::string paddr, std::string pmethod);

	virtual int httpsGETCallback(const char *Id, 
					std::string *data_str, 
					std::string sToken, 
					std::string sSignature, 
					std::string clientDistName);

	virtual size_t httpsResponseCallback(char *ptr, size_t size);

	int MakeHttpsServer(unsigned short listen_port);
	int KillHttpsServer();

};
}
