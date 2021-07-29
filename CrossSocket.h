
#include "crossSocketIncludes.h"

namespace CSWL
{

	class CrossSocket
	{
		SocketType type;

		AddressFamily family;

		IpProtocol protocol;

		ServerOrClient behaviour;

		short port;
		Endpoint endpoint;

		void* winWSADATA;

		int currentStateResult;
		std::string currentError;

		void initialisation();
		void deconstruction();

		void createSocket(std::string domainOrIp = "");



		uintptr_t crsSocket;

	public:
		//todo Rule of 3

		int translateEnum(AddressFamily);
		int translateEnum(IpProtocol);
		int translateEnum(SocketType);

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

		CrossSocket(ServerOrClient behaviour, short port, AddressFamily family, SocketType type, IpProtocol protocol)
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

		CrossSocket(ServerOrClient behaviour, short port, AddressFamily family, SocketType type, IpProtocol protocol, std::string domainOrDottedIp)
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

		AddressFamily getFamlily()
		{
			return family;
		}

		SocketType getSocketType()
		{
			return type;
		}

		IpProtocol getProtocol()
		{
			return protocol;
		}

		Endpoint getEntpoint()
		{
			return endpoint;
		}



	};

}

//Implementation

#include "resolverImplementation.h"

#include "crossSocketImplementation.h"








