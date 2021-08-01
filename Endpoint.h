#include "IpAddress.h"
namespace CSWL
{
	class Endpoint
	{

	public:

		IpAddress ip;
		unsigned short port;

		Endpoint()
		{
			port = 0;
		}

		Endpoint(const Endpoint& other)
		{
			*this = other;
		}

		Endpoint& operator=(const Endpoint& other)
		{
			this->port = other.port;
			this->ip = other.ip;
			return *this;
		}

		
		///
		/**
		* Return like 192.172.0.1:22 or [aaaa:bbbb:...]:80
		*/
		std::string toString()
		{
			std::string result = "";
			if (ip.getVersion() == CSWL::IpVersion::IPv4)
			{
				result = ip.toString() + ":" + std::to_string(port);
			}
			else
			{
				result =  "[" + ip.toString() + "]:" + std::to_string(port);
			}
			return result;
		}


	};
}

//....


