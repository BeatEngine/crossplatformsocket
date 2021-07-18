#include <string>
namespace CSWL
{
	enum IpVersion
	{
		IPv4, IPv6
	};

	enum AddressFamily
	{
		//LINUX NAMES
		CS_AF_INET, CS_AF_INET6, CS_AF_IRDA, CS_AF_BLUETOOTH
		//WINDOWS
		// AF_BLUETOOTH = AF_BTH
	};

	enum SocketType
	{
		//LINUX NAMES:
		CS_SOCK_STREAM, CS_SOCK_DGRAM, CS_SOCK_RAW, CS_SOCK_SEQPACKET
	};

	enum IpProtocol
	{
		CS_IPPROTO_ICMP, CS_IPPROTO_IGMP, CS_IPPROTO_TCP, CS_IPPROTO_UDP
	};

	class IpAddress
	{
		IpVersion version;
		//Contains V4 or V6
		unsigned char addressData[16];
	public:

		IpAddress()
		{
			addressData[0] = 0;
			version = IPv4;
		}

		/**
		*	@Param commonIpRepresentation 254.254.254.254 or aaaa:bbbb:cccc  ...-->
		*
		*/
		IpAddress(std::string& commonIpRepresentation)
		{

		}

		//todo Rule of 3

		/**
		* @Return String representation of Ip (v4 xor v6)
		*/
		std::string toString()
		{
			//todo impl
		}

	};
}
