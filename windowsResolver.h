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
		sprintf(tmpPort, "%d", port);
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

	std::vector<CSWL::Endpoint> resolveAddresses(std::string domainOrDottedIp, short port, CrossSocket& socket)
	{
		struct addrinfo* result = 0;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = socket.translateEnum(socket.getFamlily());
		hints.ai_socktype = socket.translateEnum(socket.getSocketType());
		hints.ai_protocol = socket.translateEnum(socket.getProtocol());
		hints.ai_flags = AI_PASSIVE;
		char tmpPort[7];
		sprintf(tmpPort, "%d", port);

		std::vector<Endpoint> endpoints;

		int iResult = getaddrinfo(domainOrDottedIp.c_str(), tmpPort, &hints, &result);
		if (iResult != 0)
		{
			std::string err = "getaddrinfo failed with error: ";
			err += std::to_string(iResult);
			socket.setError(err);
		}
		else
		{
			CSWL::IpVersion version = CSWL::IpVersion::IPv4;
			if (socket.getFamlily() == CSWL::AddressFamily::CS_AF_INET6)
			{
				version = CSWL::IpVersion::IPv6;
			}

			struct addrinfo* currentResult = result;
			do
			{
				Endpoint endpoint;
				endpoint.port = port;
				// Copy Ip binary into Endpoint

				if (!endpoint.ip.setBinaryData(currentResult->ai_addr->sa_data, currentResult->ai_addrlen, version))
				{
					std::string err = "converting binary resolver result failed.";
					socket.setError(err);
				}

				endpoints.push_back(endpoint);

				currentResult = currentResult->ai_next;
			} while (currentResult != 0);
		}
		freeaddrinfo(result);
		return endpoints;
	}

}

