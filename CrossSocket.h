
#ifdef __linux__ 
	#include <unistd.h>
	#include <sys/types.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
#else
	#define _CRT_SECURE_NO_WARNINGS
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
	#pragma comment (lib, "AdvApi32.lib")
#endif

#include <vector>

#include "WLthread.h"
#include "Endpoint.h"

class CrossSocket
{
	CSWL::SocketType type;

	CSWL::AddressFamily family;

	CSWL::IpProtocol protocol;

	CSWL::ServerOrClient behaviour;

	short port;
	CSWL::Endpoint endpoint;

	void* winWSADATA;

	int currentStateResult;
	std::string currentError;

	void initialisation();
	void deconstruction();

	void createSocket(std::string domainOrIp = "");
	


	uintptr_t crsSocket;

public:
	//todo Rule of 3

	int translateEnum(CSWL::AddressFamily);
	int translateEnum(CSWL::IpProtocol);
	int translateEnum(CSWL::SocketType);

	void setError(std::string error)
	{
		currentError = error;
	}

	int connect();

	int bind();
	int listen();
	int accept();

	int receive();

	int send();

	int shutdown();

	/**
	* @Return true if no error
	*/
	bool actionSuccess()
	{
		return currentError.size() == 0;
	}

	/*
	* @Return last Error message.
	*/
	std::string lastError()
	{
		return currentError;
	}

	/**
	* Delete last Error
	*/
	void unsetError()
	{
		currentError = "";
	}

	CrossSocket(CSWL::ServerOrClient behaviour, short port, CSWL::AddressFamily family, CSWL::SocketType type, CSWL::IpProtocol protocol)
	{
		crsSocket = 0;
		currentStateResult = 0;
		winWSADATA = 0;
		currentError = "";
		this->port = port;
		this->behaviour = behaviour;
		this->family = family;
		this->type = type;
		this->protocol = protocol;
		initialisation();
		if (actionSuccess())
		{
			createSocket();
		}
	}

	CrossSocket(CSWL::ServerOrClient behaviour, short port, CSWL::AddressFamily family, CSWL::SocketType type, CSWL::IpProtocol protocol, std::string domainOrDottedIp)
	{
		crsSocket = 0;
		currentStateResult = 0;
		winWSADATA = 0;
		currentError = "";
		this->port = port;
		this->behaviour = behaviour;
		this->family = family;
		this->type = type;
		this->protocol = protocol;
		initialisation();
		if (actionSuccess())
		{
			createSocket(domainOrDottedIp);
		}
	}

	~CrossSocket()
	{
		deconstruction();
	}

	CSWL::AddressFamily getFamlily()
	{
		return family;
	}

	CSWL::SocketType getSocketType()
	{
		return type;
	}

	CSWL::IpProtocol getProtocol()
	{
		return protocol;
	}

	CSWL::Endpoint getEntpoint()
	{
		return endpoint;
	}



};


//Implementation
#include "Resolver.h"

#ifdef __linux__ 
#include "linuxImplementation.h"
#else
#include "windowsImplementation.h"
#endif






