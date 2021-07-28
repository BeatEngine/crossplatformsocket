
namespace CSWL
{
	class Resolver
	{

	public:


		static Endpoint resolveLocalServerAddress(short port, CrossSocket& socket);

		static std::vector<CSWL::Endpoint> resolveAddresses(std::string domainOrDottedIp, short port, CrossSocket& socket);




	};


}

#ifdef __linux__ 
#include "linuxResolver.h"
#else
#include "windowsResolver.h"
#endif






