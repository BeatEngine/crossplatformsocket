#include <vector>

#ifdef __linux__ 
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include <stdio.h>
#include <string.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define WSAGetLastError() errno
#define SOCKET_ERROR 0
#define SOCKADDR sockaddr
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
//Maybe later
//#pragma comment (lib, "AdvApi32.lib")
#endif

//TLS for secure communication
//
//

//Maybe for integration of thread usage
//#include "WLthread.h"
#include "Endpoint.h"
#include "Resolver.h"

//....

