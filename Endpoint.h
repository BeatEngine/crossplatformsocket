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

		//todo Rule of 3


	};
}
