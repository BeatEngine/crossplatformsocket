namespace CSWL
{
	Endpoint Resolver::resolveLocalServerAddress(short port, CrossSocket& socket)
	{
		Endpoint endpoint;
		struct addrinfo* result = 0;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = socket.translateEnum(socket.getFamlily());
		hints.ai_socktype = socket.translateEnum(socket.getSocketType());
		hints.ai_protocol = socket.translateEnum(socket.getProtocol());
		hints.ai_flags = AI_PASSIVE;
		char tmpPort[7];
		sprintf_s(tmpPort, "%d", port);
		int iResult = getaddrinfo(NULL, tmpPort, &hints, &result);
		if (iResult != 0)
		{
			std::string err = "getaddrinfo failed with error: ";
			err += std::to_string(iResult);
			socket.setError(err);
		}
		else
		{
			endpoint.port = port;
			// Copy Ip binary into Endpoint

			CSWL::IpVersion version = CSWL::IpVersion::IPv4;
			if (socket.getFamlily() == CSWL::AddressFamily::CS_AF_INET6)
			{
				version = CSWL::IpVersion::IPv6;
			}
			
			if (!endpoint.ip.setBinaryData(result->ai_addr->sa_data, result->ai_addrlen, version))
			{
				std::string err = "converting binary resolver result failed.";
				socket.setError(err);
			}
		}
		freeaddrinfo(result);
		return endpoint;
	}

	Endpoint Resolver::resolveAddress(std::string domainOrDottedIp, short port, CrossSocket& socket)
	{
		Endpoint endpoint;
		struct addrinfo* result = 0;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = socket.translateEnum(socket.getFamlily());
		hints.ai_socktype = socket.translateEnum(socket.getSocketType());
		hints.ai_protocol = socket.translateEnum(socket.getProtocol());
		hints.ai_flags = AI_PASSIVE;
		char tmpPort[7];
		sprintf_s(tmpPort, "%d", port);
		int iResult = getaddrinfo(domainOrDottedIp.c_str(), tmpPort, &hints, &result);
		if (iResult != 0)
		{
			std::string err = "getaddrinfo failed with error: ";
			err += std::to_string(iResult);
			socket.setError(err);
		}
		else
		{
			endpoint.port = port;
			// Copy Ip binary into Endpoint

			CSWL::IpVersion version = CSWL::IpVersion::IPv4;
			if (socket.getFamlily() == CSWL::AddressFamily::CS_AF_INET6)
			{
				version = CSWL::IpVersion::IPv6;
			}

			if (!endpoint.ip.setBinaryData(result->ai_addr->sa_data, result->ai_addrlen, version))
			{
				std::string err = "converting binary resolver result failed.";
				socket.setError(err);
			}
		}
		freeaddrinfo(result);
		return endpoint;
	}

}