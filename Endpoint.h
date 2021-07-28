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


	};
}
