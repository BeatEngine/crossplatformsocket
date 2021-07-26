

namespace CSWL
{
	class Resolver
	{

	public:


		static Endpoint resolveLocalServerAddress(short port, CrossSocket& socket);

		static Endpoint resolveAddress(std::string domainOrDottedIp, short port, CrossSocket& socket);
		



	};
}

#ifdef __linux__ 
#include "linuxResolver.h"
#else
#include "windowsResolver.h"
#endif


