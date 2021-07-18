#include "WLthread.h";
#include "Endpoint.h"
#include "Resolver.h"

class CrossSocket
{
	CSWL::SocketType type;

	CSWL::AddressFamily family;

	CSWL::IpProtocol protocol;


	void* winWSADATA;

	int currentStateResult;
	std::string currentError;

	void initialisation();
	void deconstruction();

	int translateEnum(CSWL::AddressFamily);
	int translateEnum(CSWL::IpProtocol);
	int translateEnum(CSWL::SocketType);

	void createSocket();

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


	uintptr_t crsSocket;

public:
	//todo Rule of 3

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


	CrossSocket()
	{
		crsSocket = 0;
		currentStateResult = 0;
		winWSADATA = 0;
		currentError = "";
		family = CSWL::AddressFamily::CS_AF_INET;
		type = CSWL::SocketType::CS_SOCK_STREAM;
		protocol = CSWL::IpProtocol::CS_IPPROTO_TCP;
		initialisation();
	}

	CrossSocket(CSWL::AddressFamily family, CSWL::SocketType type, CSWL::IpProtocol protocol)
	{
		crsSocket = 0;
		currentStateResult = 0;
		winWSADATA = 0;
		currentError = "";
		this->family = family;
		this->type = type;
		this->protocol = protocol;
		initialisation();
	}

	~CrossSocket()
	{
		deconstruction();
	}




};


#ifdef __linux__ 
#include "linuxImplementation.h"
#else
#include "windowsImplementation.h"
#endif



