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

	enum ServerOrClient
	{
		SERVER, CLIENT
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
			setFormatedData(commonIpRepresentation);
		}


		IpAddress& operator=(const IpAddress& other)
		{
			this->version = other.version;
			for (int i = 0; i < 16; i++)
			{
				addressData[i] = other.addressData[i];
			}
			return *this;
		}

		IpAddress(const IpAddress& other)
		{
			*this = other;
		}

		bool setBinaryData(char* data, int dataLength, IpVersion version)
		{
			this->version = version;
			if (dataLength < 17 && data != 0 && dataLength > 0)
			{
				for (int i = 0; i < dataLength; i++)
				{
					addressData[i] = data[i];
				}
			}
			else
			{
				return false;
			}
			return true;
		}

		bool setFormatedData(std::string data)
		{
			
			if (data.size() < 44 && data.size() > 0)
			{
				std::string tmp = "";
				int positionData = 0;
				if (data.find('.') != std::string::npos)
				{
					this->version = CSWL::IpVersion::IPv4;
					for (int i = 0; i < data.size(); i++)
					{
						if (data[i] == '.')
						{
							int dez = atoi(tmp.c_str());
							addressData[positionData] = (unsigned char)(dez);
							positionData++;
							tmp = "";
						}
						else
						{
							tmp += data[i];
						}
					}
					int dez = atoi(tmp.c_str());
					addressData[positionData] = (unsigned char)(dez);
				}
				else if (data.find(':') != std::string::npos)
				{
					this->version = CSWL::IpVersion::IPv6;
					for (int i = 0; i < data.size(); i++)
					{
						if (data[i] == ':')
						{
							unsigned int conv = 0;
							sscanf(tmp.c_str(), "%x", &conv);
							addressData[positionData] = (unsigned char)(conv >> 8);
							addressData[positionData+1] = (unsigned char)(conv);
							positionData+=2;
							tmp = "";
						}
						else
						{
							tmp += data[i];
						}
					}
					unsigned int conv = 0;
					sscanf(tmp.c_str(), "%x", &conv);
					addressData[positionData] = (unsigned char)(conv >> 8);
					addressData[positionData + 1] = (unsigned char)(conv);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}


		/**
		* @Return String representation of Ip (v4 ^ v6)
		*/
		std::string toString()
		{
			std::string result = "";
			if (version == IpVersion::IPv4)
			{
				for (int i = 0; i < 4; i++)
				{
					result += std::to_string(addressData[i]);
					if (i + 1 < 4)
					{
						result += ".";
					}
				}
			}
			else
			{
				char hex[5] = "0000";
				for (int i = 0; i < 16; i += 2)
				{
					//Two bytes into one 2 bytes long short first or second part e.g. (1010101100000000 | 0000000010101010)
					unsigned short tmp = ((unsigned short)(addressData[i]) << 8) | (unsigned short)(addressData[i + 1]);
					sprintf(hex, "%x", tmp);
					std::string zeroFilled = hex;
					while (zeroFilled.size() < 4)
					{
						zeroFilled = "0" + zeroFilled;
					}
					result += zeroFilled;
					if (i + 2 < 16)
					{
						result += ":";
					}
				}
			}
			return result;
		}

	};
}
