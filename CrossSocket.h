
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

		void createSocket(std::string domainOrIp = "");

		void bindCS();
		void listenCS();



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

		int connectCS();

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

			this->winWSADATA = other.winWSADATA;

			return *this;
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








