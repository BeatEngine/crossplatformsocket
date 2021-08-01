#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif

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

		std::vector<Endpoint> possibleEndpoints;

		void* winWSADATA;

		int currentStateResult;
		std::string currentError;

		void initialisation();
		void deconstruction();

		int receiveTimeoutMilli = 800;

		void createSocket(std::string domainOrIp = "");

		void bindCS();
		void listenCS();

		//void* contextTLS = 0;
		/*void initialiseTLS(CSWL::ServerOrClient type)
		{
			if (useTLS)
			{
				
			}
		}
		bool useTLS = false;
		void deconstructTLS()
		{
			if (useTLS)
			{


			}
		}*/

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

		/// <summary>
		/// Connect to domain or formatted IP V4 "192.168.178.77" or V6 "aaaa:bbbb:cccc:7777:...."
		/// </summary>
		/// <param name="domainOrIp">"192.168.178.77" or "aaaa:bbbb:cccc:7777:...."</param>
		/// <returns>actionSuccess()</returns>
		bool connectCS(std::string domainOrIp, bool directDNS = false);

		bool connectCS(IpAddress address);

		CrossSocket acceptCS();

		/// <summary>
		/// Receive data and store into buffer
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="bufferSize"></param>
		/// <returns>Size received in bytes</returns>
		int receiveCS(unsigned char* buffer, int bufferSize);

		int sendCS(const unsigned char* buffer, int bufferSize);

		int shutdownCS();

		///
		/**
		* @Return true if no error
		*/
		bool actionSuccess()
		{
			return currentError.size() == 0;
		}

		///
		/*
		* @Return last Error message.
		*/
		std::string lastError()
		{
			return currentError;
		}

		///
		/**
		* Delete last Error
		*/
		void unsetError()
		{
			currentError = "";
		}

		CrossSocket(std::string error)
		{
			currentError = error;
			if (currentError.empty())
			{
				currentError = "ERROR NOT SET";
			}
		}

		CrossSocket(ServerOrClient behaviour, short port, AddressFamily family, SocketType type, IpProtocol protocol, int receiveTimeoutMilli = 800)
		{
			crsSocket = 0;
			currentStateResult = 0;
			winWSADATA = 0;
			currentError = "";
			this->receiveTimeoutMilli = receiveTimeoutMilli;
			this->port = port;
			this->behaviour = behaviour;
			this->family = family;
			this->type = type;
			this->protocol = protocol;
			//initialiseTLS(behaviour);
			initialisation();
			if (actionSuccess())
			{
				createSocket();
			}
		}

		/// <summary>
		/// For Client socket
		/// </summary>
		/// <param name="behaviour"></param>
		/// <param name="port"></param>
		/// <param name="family"></param>
		/// <param name="type"></param>
		/// <param name="protocol"></param>
		/// <param name="domainOrDottedIp"></param>
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
			//initialiseTLS(behaviour);
			initialisation();
			if (actionSuccess())
			{
				createSocket(domainOrDottedIp);
			}
		}

		/// <summary>
		/// For Server accept Client construction.
		/// </summary>
		/// <param name="behaviour"></param>
		/// <param name="port"></param>
		/// <param name="family"></param>
		/// <param name="type"></param>
		/// <param name="protocol"></param>
		CrossSocket(ServerOrClient behaviour, short port, AddressFamily family, SocketType type, IpProtocol protocol, Endpoint endpoint, uintptr_t acceptedSocketHandle)
		{
			crsSocket = acceptedSocketHandle;
			currentStateResult = 0;
			winWSADATA = 0;
			currentError = "";
			this->port = port;
			this->behaviour = behaviour;
			this->family = family;
			this->type = type;
			this->protocol = protocol;
			this->endpoint = endpoint;
			//initialiseTLS(behaviour);
			initialisation();
		}

		CrossSocket(const CrossSocket& other)
		{
			*this = other;
		}

		CrossSocket& operator=(const CrossSocket& other)
		{
			this->behaviour = other.behaviour;
			this->crsSocket = other.crsSocket;
			this->currentError = other.currentError;
			this->currentStateResult = other.currentStateResult;
			this->endpoint = other.endpoint;
			this->family = other.family;
			this->port = other.port;
			this->protocol = other.protocol;
			this->type = other.type;

			/*if (contextTLS != 0)
			{
				free(contextTLS);
			}*/
			//todo allocate and copy specific structure into contextTLS
			/*if (other.behaviour == CSWL::ServerOrClient::SERVER)
			{

			}
			else
			{

			}*/
			

			this->winWSADATA = other.winWSADATA;

			return *this;
		}

		~CrossSocket()
		{
			//deconstructTLS();
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








