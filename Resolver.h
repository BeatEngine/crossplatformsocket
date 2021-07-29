
namespace CSWL
{
	class CrossSocket;
	class Resolver
	{

	public:


		static std::vector<Endpoint> resolveLocalServerAddresses(short port, CrossSocket& socket);

		static std::vector<Endpoint> resolveAddresses(std::string domainOrDottedIp, short port, CrossSocket& socket);




	};


}


