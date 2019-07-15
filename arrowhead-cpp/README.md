# Arrowhead framework client C++  
These is a library to enable the use of Arrowhead framework with `c++`
code.
It is designed to be installed ass a library.

## Prerequisites
The following Linux packages are prerequisities:
openssl, libgnutls28-dev, libgnutlsxx28, libssl1.1, libssl1.0-dev, libcurl3, libcurl3-gnutls, libcurl4-gnutls-dev, libcrypto++-dev, libcrypto++-utils, libcrypto++6, libgpg-error-dev, automake, texinfo, g++, libjson-c-dev

The project uses libmicrohttpd-0.9.59 as well. Download, compile and install it from source with HTTPS support: https://ftp.gnu.org/gnu/libmicrohttpd/
  >tar -xvzf libmicrohttpd-0.9.59.tar.gz

  >./configure --with-gnutls

  >make

  >sudo make install

Create libmicrohttpd.so.12 file in /usr/lib or usr/local/lib directory (or where the ”ldd ProviderExample” command points):
  >cd /usr/lib

  >sudo ln –s /usr/local/lib/libmicrohttpd.so.12.46.0 libmicrohttpd.so.12

In a case of missing libcrypto.so:
>	cd /usr/lib

## Installation
To install de library clone these project.
Navigate to the source folder and run:

> cmake CMakeLists.txt  
> sudo make install


## Usages
To use the library you need to include the library in your build path.
To do sow add the following to your `CMakeLists.txt`
```
set(PROJECT_LINK_LIBS libarrowhead_cpp.so)
link_directories(/usr/local/lib/)
target_link_libraries(${your_project_name} ${PROJECT_LINK_LIBS})
```
In the code do you include the type of Arrowhead system you desire:
provider, consumer, publisher or subscriber.
```cpp
#include "arrowhead/Provider.h"
#include "arrowhead/Consumer.h"
#include "arrowhead/Publisher.h"
#include "arrowhead/Subscriber.h"
```
It is written in `namespace arrowhead`.
You can off curse include ass many ass you need.
### Provider example
```cpp
Provider my_provider;
my_provider.config /.../
my_provider.init("a identefiing name");
my_provider.setMsgs(json_object)
```
`my_provider.config` is a struct containing settings, see more under
`config`.
The `json_object` shod contain the msgs you providing.

### Consumer example
```cpp
Consumer my_consumer;
my_consumer.config /.../
my_consumer.init(callbackFunction);
my_consumer.request();
```
The callbackFunction have to bi a `static void` whit parameter `const
char* sendigng_url, const char* msgs)`

### Publisher example
```cpp
Publisher my_publisher;
my_publisher.config /.../
my_publisher.init();
my_publisher.publish(json_object);
```

### Subscriber example
```cpp
Subscriber my_subscriber;
my_subscriber.config /.../
my_subscriber.init("a identefining name", callbackFunction);
```

### config
Arrowhead framework require some system specific parameters to work.
Those parameters are set by the `.config` struct.
The parameters need to be set before the `init` function.
It is recommended to create some launch file where these parameters are
define and then loaded in code.
Many of these parameters need to sync with the rest of the system and you
only want to set the parameter once.
It is left to you to figure out how to do that best in your
system.

The parameters that are to be set are: (all is not always used)


| Parameter name 				| Type 		| Usages |
|:------------------------------|:----------|:-------|
| SERVICE_DEFINITION			| string	| Name of the service |
| INTERFACE						| string	| Interface used ex. JSON |
| SERVICE_URI					| string	| Identification string |
| UNIT							| string	| The unit of the service data |
| SECURITY						| string	| Type of security ex. Token |
| ACCESS_URI					| string	| URI to serviseRegestry or
orchestration |
| ACCESS_URI_HTTPS				| string	| HTTPs URI serviseReqestry or
orchestration |
| OVERRIDE_STORE				| bool		| Orchestration flags |
| MATCHMAKING					| bool		| Orchestration flags |
| PING_PROVIDERS				| bool		| Orchestration flags |
| ONLY_PREFERRED				| bool		| Orchestration flags |
| EXTERNAL_SERVICE_REQUEST		| bool		| Orchestration flags |
| THIS_SYSTEM_NAME				| string	| This systems name |
| THIS_ADDRESS					| string 	| This system's ipv4 address|
| THIS_ADDRESS6					| string 	| This system's ipv6 address|
| THIS_PORT						| int		| This system's port |
| TARGET_SYSTEM_NAME			| string	| Target system's name |
| TARGET_ADDRESS				| string	| Target system's ipv4 address|
| TARGET_PORT 					| int		| Target system's port |
| SECURE_ARROWHEAD_INTERFACE	| bool		| Is HTTPs used |
| SECURE_PROVIDER_INTERFACE		| bool		| Is HTTPs used to interface
provider |
| PUBLIC_KEY_PATH				| string	| Absolute path to public key |
| PRIVATE_KEY_PATH				| string	| Absolute path to private key |
| AUTHENTICATION_INFO			| string	| not user set |


# Work to be don
## HTTPs
At the moment is no HTTPs functionality implemented.
Some code for that do exist.
The existing HTTPs code (like with HttpsHandler and HttpHandler) need to be split from non HTTP code to create have
a good code structure.

## Code robustness
There is a need for some checks on in and outgoing msgs sow it is the
correct once with correct formatting.

## File reading constructor
To have the option of a constructor that tack a file (preferably .json) and load parameters form
would be good.

