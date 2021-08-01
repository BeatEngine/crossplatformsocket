namespace CSWL
{

    void CrossSocket::initialisation()
    {
        //Not needed
    }

    void CrossSocket::deconstruction()
    {
        if (crsSocket)
        {
            close(crsSocket);
        }
    }

    int CrossSocket::translateEnum(CSWL::AddressFamily fam)
    {
        if (fam == CSWL::AddressFamily::CS_AF_INET)
        {
            return AF_INET;
        }
        if (fam == CSWL::AddressFamily::CS_AF_INET6)
        {
            return AF_INET6;
        }
        if (fam == CSWL::AddressFamily::CS_AF_IRDA)
        {
            return AF_IRDA;
        }
        if (fam == CSWL::AddressFamily::CS_AF_BLUETOOTH)
        {
            return AF_BLUETOOTH;
        }
        setError("ENUM translation (family) failed!!!");
        return -1;
    }

    int CrossSocket::translateEnum(CSWL::IpProtocol prot)
    {
        if (prot == CSWL::IpProtocol::CS_IPPROTO_TCP)
        {
            return IPPROTO_TCP;
        }
        if (prot == CSWL::IpProtocol::CS_IPPROTO_UDP)
        {
            return IPPROTO_UDP;
        }
        if (prot == CSWL::IpProtocol::CS_IPPROTO_ICMP)
        {
            return IPPROTO_ICMP;
        }
        if (prot == CSWL::IpProtocol::CS_IPPROTO_IGMP)
        {
            return IPPROTO_IGMP;
        }
        setError("ENUM translation (protocol) failed!!!");
        return -1;
    }

    int CrossSocket::translateEnum(CSWL::SocketType typ)
    {
        if (typ == CSWL::SocketType::CS_SOCK_STREAM)
        {
            return SOCK_STREAM;
        }
        if (typ == CSWL::SocketType::CS_SOCK_DGRAM)
        {
            return SOCK_DGRAM;
        }
        if (typ == CSWL::SocketType::CS_SOCK_RAW)
        {
            return SOCK_RAW;
        }
        if (typ == CSWL::SocketType::CS_SOCK_SEQPACKET)
        {
            return SOCK_SEQPACKET;
        }
        setError("ENUM translation (type) failed!!!");
        return -1;
    }


/// <summary>
    /// Create a socket
    /// </summary>
    /// <param name="domainOrIp">Connect directly if not empty.</param>
    void CrossSocket::createSocket(std::string domainOrIp)
    {
        if (this->behaviour == ServerOrClient::SERVER)
        {
            //Server
            possibleEndpoints = Resolver::resolveLocalServerAddresses(this->port, *this);
            if (!this->actionSuccess())
            {
                //Error was set by resolver.
                return;
            }
            endpoint = possibleEndpoints[0];
            SOCKET sock = socket(translateEnum(this->family), translateEnum(this->type), translateEnum(this->protocol));
            if (sock == INVALID_SOCKET) {

                std::string err = "create socket result: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return;
            }

            struct timeval timeout;      
            timeout.tv_sec = 0;
            timeout.tv_usec = receiveTimeoutMilli;
            
            if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
            {
                std::string err = "set socket option: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return;
            }

            const int y = 1;
            if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) < 0)
            {
                std::string err = "set socket option: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return;
            }

            crsSocket = sock;
            
            bindCS();
            if (actionSuccess())
            {
                listenCS();
            }
        }
        else
        {
            //Client
            SOCKET sock = socket(translateEnum(this->family), translateEnum(this->type), translateEnum(this->protocol));
            if (sock == INVALID_SOCKET)
            {
                std::string err = "create socket result: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return;
            }
            crsSocket = sock;
            if (domainOrIp.length() > 0)
            {
                //This sets error on fail.
                connectCS(domainOrIp);
            }
        }
    }

    

    void CrossSocket::bindCS()
    {
        //WARNING IPV4 use different structs than IPV6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        SOCKET socket = crsSocket;
        sockaddr sap;
        sockaddr_in6 sap6;
        bool useV6 = false;
        for (int e = 0; e < possibleEndpoints.size(); e++)
        {
            if (endpoint.ip.getVersion() != IpVersion::IPv4)
            {
                useV6 = true;
            }
            char* rData = endpoint.ip.rawData();
            if (useV6)
            {
                sap6.sin6_family = translateEnum(family);
                sap6.sin6_port = port;
                for (int i = 0; i < 16; i++)
                {
                    sap6.sin6_addr.__in6_u.__u6_addr8[i] = rData[i];
                }
                currentStateResult = bind(socket, (SOCKADDR*)&sap6, sizeof(sap6));
            }
            else
            {
                for(int i = 0; i < 14; i++)
                {
                    sap.sa_data[i] = rData[i];
                }
                /*sap.sin_addr.s_addr = (((unsigned int)rData[0]) << 24)
                                    | (((unsigned int)rData[1]) << 16)
                                    | (((unsigned int)rData[2]) << 8)
                                    | ((unsigned int)rData[3]);*/
                //sap.sin_port = port;
                sap.sa_family = translateEnum(family);
                currentStateResult = bind(socket, (SOCKADDR*)&sap, endpoint.ip.rawLength());
            }

            if (currentStateResult != 0)
            {
                int errCode = WSAGetLastError();
                if (e + 1 < possibleEndpoints.size())
                {
                    if (errCode == 10049 || errCode == 10048)
                    {
                        // Not available or in use
                        continue;
                    }
                }
                std::string err = "bind failed with error: ";
                err += std::to_string(errCode);
                setError(err);
                return;
            }
        }
    }
    void CrossSocket::listenCS()
    {
        SOCKET socket = crsSocket;
        currentStateResult = listen(socket, SOMAXCONN);
        if (currentStateResult != 0)
        {
            std::string err = "listen failed with error: ";
            err += std::to_string(WSAGetLastError());
            if(WSAGetLastError() == 13)
            {
                err += " (no permissions)";
            }
            setError(err);
            return;
        }
    }


    CrossSocket CrossSocket::acceptCS()
    {
        SOCKET socket = crsSocket;
        sockaddr_in  clientAddr4;
        sockaddr_in6 clientAddr6;
        unsigned int adrLength = 0;
        bool useV6 = false;
        //WARNING IPV4 use different structs than IPV6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        SOCKET client;
        while(true)
        {
            if (endpoint.ip.getVersion() == IpVersion::IPv4)
            {
                adrLength = sizeof(clientAddr4);
                client = accept(socket, (SOCKADDR*)&clientAddr4, &adrLength);
            }
            else
            {
                adrLength = sizeof(clientAddr6);
                client = accept(socket, (SOCKADDR*)&clientAddr6, &adrLength);
                useV6 = true;
            }
            if (client == INVALID_SOCKET)
            {
                if(WSAGetLastError() == 11)
                {
                    //timeout
                    continue;
                }
                std::string err = "accept failed with error: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return CrossSocket(err);
            }
            break;
        }
        Endpoint endp;

        IpVersion clientIpVersion = IpVersion::IPv4;
        if (useV6)
        {
            clientIpVersion = IpVersion::IPv6;
            endp.ip.setBinaryData((char*)&(clientAddr6.sin6_addr.__in6_u.__u6_addr8), 16, clientIpVersion);
        }
        else
        {
            endp.ip.setBinaryData((char*)&(clientAddr4.sin_addr.s_addr), sizeof(clientAddr4.sin_addr.s_addr), clientIpVersion);
        }
        
        endp.port = port;
        
        return CrossSocket(CSWL::ServerOrClient::CLIENT, port, family, type, protocol, endp, client);
    }

    bool CrossSocket::connectCS(std::string domainOrIp, bool directDNS)
    {
        possibleEndpoints = Resolver::resolveAddresses(domainOrIp, this->port, *this);
        if (!this->actionSuccess())
        {
            //Error was set by resolver.
            return false;
        }
        SOCKET client = crsSocket;
        sockaddr conAddrV4;
        sockaddr_in6 conAddrV6;
        for (int i = 0; i < possibleEndpoints.size(); i++)
        {
            Endpoint endpt = possibleEndpoints[i];
            char* rawAddr = endpt.ip.rawData();

            if (endpt.ip.getVersion() == IpVersion::IPv6)
            {
                conAddrV6.sin6_family = translateEnum(family);
                conAddrV6.sin6_port = endpt.port;
                for (int r = 0; r < 16; r++)
                {
                    conAddrV6.sin6_addr.__in6_u.__u6_addr8[r] = rawAddr[r];
                }
                currentStateResult = connect(client, (SOCKADDR*)&conAddrV6, sizeof(conAddrV6.sin6_addr));
            }
            else
            {
                conAddrV4.sa_family = translateEnum(family);
                for (int r = 0; r < 14; r++)
                {
                    conAddrV4.sa_data[r] = rawAddr[r];
                }
                currentStateResult = connect(client, &conAddrV4, sizeof(conAddrV4));
            }

            if (currentStateResult == SOCKET_ERROR) {
                if(WSAGetLastError() == 11)
                {
                    //timeout
                    i--;
                    continue;
                }
                if (i + 1 < possibleEndpoints.size())
                {
                    continue;
                }
                else
                {
                    std::string err = "connect failed with error: ";
                    err += std::to_string(WSAGetLastError());
                    setError(err);
                    return false;
                }
            }
            endpoint = endpt;
            break;
        }
        return true;
    }

    bool CrossSocket::connectCS(IpAddress address)
    {
        SOCKET client = crsSocket;
        sockaddr conAddrV4;
        sockaddr_in6 conAddrV6;
        Endpoint endpt;
        endpt.ip = address;
        endpt.port = port;
        char* rawAddr = endpt.ip.rawData();

        if (endpt.ip.getVersion() == IpVersion::IPv6)
        {
            conAddrV6.sin6_family = translateEnum(family);
            conAddrV6.sin6_port = endpt.port;
            for (int r = 0; r < 16; r++)
            {
                conAddrV6.sin6_addr.__in6_u.__u6_addr8[r] = rawAddr[r];
            }
            currentStateResult = connect(client, (SOCKADDR*)&conAddrV6, sizeof(conAddrV6.sin6_addr));
        }
        else
        {
            conAddrV4.sa_family = translateEnum(family);
            for (int r = 0; r < 14; r++)
            {
                conAddrV4.sa_data[r] = rawAddr[r];
            }
            currentStateResult = connect(client, &conAddrV4, sizeof(conAddrV4));
        }

        if (currentStateResult == SOCKET_ERROR)
        {
            std::string err = "connect failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return false;
        }
        endpoint = endpt;
        return true;
    }

    int CrossSocket::receiveCS(unsigned char* buffer, int bufferSize)
    {
        if (buffer == 0 || bufferSize < 1)
        {
            if (buffer == 0)
            {
                setError("receive buffer is null.");
            }
            else
            {
                setError("receive buffer size less 1.");
            }
            return 0;
        }
        char* buff = (char*)buffer;
        SOCKET clientSocket = crsSocket;
        currentStateResult = recv(clientSocket, buff, bufferSize, 0);
        //currentStateResult = read(clientSocket, buff, bufferSize);
        if (currentStateResult < 0)
        {
            std::string err = "receive failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return 0;
        }
        return currentStateResult;

    }

    int CrossSocket::sendCS(const unsigned char* buffer, int bufferSize)
    {
        if (buffer == 0 || bufferSize < 1)
        {
            if (buffer == 0)
            {
                setError("send buffer is null.");
            }
            else
            {
                setError("send buffer size less 1.");
            }
            return 0;
        }
        const char* buff = (const char*)buffer;
        SOCKET clientSocket = crsSocket;
        currentStateResult = send(clientSocket, buff, bufferSize, 0);
        if (currentStateResult == SOCKET_ERROR)
        {
            std::string err = "send failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return 0;
        }
        return currentStateResult;
    }

    int CrossSocket::shutdownCS()
    {
        return 0;

    }
}

//....

