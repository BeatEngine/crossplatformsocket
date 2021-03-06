namespace CSWL
{
	
	std::vector<Endpoint> Resolver::resolveLocalServerAddresses(short port, CrossSocket& socket)
	{
		std::vector<Endpoint> endpoints;
		struct addrinfo* result = 0;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = socket.translateEnum(socket.getFamlily());
		hints.ai_socktype = socket.translateEnum(socket.getSocketType());
		hints.ai_protocol = socket.translateEnum(socket.getProtocol());
		hints.ai_flags = AI_PASSIVE;
		char tmpPort[7];
		sprintf(tmpPort, "%d", port);
		int iResult = getaddrinfo(0, tmpPort, &hints, &result);
		if (iResult != 0)
		{
			std::string err = "getaddrinfo failed with error: ";
			err += std::to_string(iResult);
			socket.setError(err);
		}
		else
		{
			IpVersion version = IpVersion::IPv4;
			if (socket.getFamlily() == AddressFamily::CS_AF_INET6)
			{
				version = IpVersion::IPv6;
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

	std::vector<Endpoint> Resolver::resolveAddresses(std::string domainOrDottedIp, short port, CrossSocket& socket)
	{
		struct addrinfo* result = 0;
		struct addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = socket.translateEnum(socket.getFamlily());
		hints.ai_socktype = socket.translateEnum(socket.getSocketType());
		hints.ai_protocol = socket.translateEnum(socket.getProtocol());
		//hints.ai_flags = AI_ALL;
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
			IpVersion version = IpVersion::IPv4;
			if (socket.getFamlily() == AddressFamily::CS_AF_INET6)
			{
				version = IpVersion::IPv6;
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

//....

