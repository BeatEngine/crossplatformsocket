
namespace CSWL
{

    void CrossSocket::deconstruction()
    {
        if (winWSADATA)
        {
            WSADATA* wsaData = (WSADATA*)winWSADATA;
            if (wsaData->lpVendorInfo)
            {
                free(wsaData->lpVendorInfo);
            }
            free(winWSADATA);
            WSACleanup();
        }
        if (crsSocket)
        {
            closesocket(crsSocket);
        }
    }

    void CrossSocket::initialisation()
    {

        WSADATA* wsaData = (WSADATA*)calloc(1, sizeof(WSADATA));
        winWSADATA = wsaData;

        currentStateResult = WSAStartup(MAKEWORD(2, 2), wsaData);
        if (currentStateResult != 0) {
            std::string err = "WSAStartup failed with result: ";
            err += std::to_string(currentStateResult);
            setError(err);
            return;
        }


    }

    int CrossSocket::translateEnum(AddressFamily fam)
    {
        if (fam == AddressFamily::CS_AF_INET)
        {
            return AF_INET;
        }
        if (fam == AddressFamily::CS_AF_INET6)
        {
            return AF_INET6;
        }
        if (fam == AddressFamily::CS_AF_IRDA)
        {
            return AF_IRDA;
        }
        if (fam == AddressFamily::CS_AF_BLUETOOTH)
        {
            return AF_BTH;
        }
        setError("ENUM translation (family) failed!!!");
        return -1;
    }

    int CrossSocket::translateEnum(IpProtocol prot)
    {
        if (prot == IpProtocol::CS_IPPROTO_TCP)
        {
            return IPPROTO_TCP;
        }
        if (prot == IpProtocol::CS_IPPROTO_UDP)
        {
            return IPPROTO_UDP;
        }
        if (prot == IpProtocol::CS_IPPROTO_ICMP)
        {
            return IPPROTO_ICMP;
        }
        if (prot == IpProtocol::CS_IPPROTO_IGMP)
        {
            return IPPROTO_IGMP;
        }
        setError("ENUM translation (protocol) failed!!!");
        return -1;
    }

    int CrossSocket::translateEnum(SocketType typ)
    {
        if (typ == SocketType::CS_SOCK_STREAM)
        {
            return SOCK_STREAM;
        }
        if (typ == SocketType::CS_SOCK_DGRAM)
        {
            return SOCK_DGRAM;
        }
        if (typ == SocketType::CS_SOCK_RAW)
        {
            return SOCK_RAW;
        }
        if (typ == SocketType::CS_SOCK_SEQPACKET)
        {
            return SOCK_SEQPACKET;
        }
        setError("ENUM translation (type) failed!!!");
        return -1;
    }

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
            crsSocket = sock;
            bindCS();
            if (actionSuccess())
            {
                listenCS();
            }
        }
        else if (domainOrIp.length() > 0)
        {
            //Client
            possibleEndpoints = Resolver::resolveAddresses(domainOrIp, this->port, *this);
            if (!this->actionSuccess())
            {
                //Error was set by resolver.
                return;
            }
            endpoint = possibleEndpoints[0];

            SOCKET sock = socket(translateEnum(this->family), translateEnum(this->type), translateEnum(this->protocol));
            if (sock == INVALID_SOCKET)
            {
                std::string err = "create socket result: ";
                err += std::to_string(WSAGetLastError());
                setError(err);
                return;
            }
            crsSocket = sock;
        }
        else
        {
            std::string err = "tried to create a clientsocket without or empty aim (domain / dotted ip).";
            setError(err);
        }
    }

    

    void CrossSocket::bindCS()
    {
        //WARNING LOOK FOR CHANGES IN MICROSOFT WINDOWS API WINSOCK2
        //WARNING IPV4 use different structs than IPV6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        SOCKET socket = crsSocket;
        sockaddr sap; // sockaddr_in not working in the way I have tried.
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
                    sap6.sin6_addr.u.Byte[i] = rData[i];
                }
                currentStateResult = bind(socket, (SOCKADDR*)&sap6, sizeof(sap6));
            }
            else
            {
                for (int trick = 0; trick < 14; trick++)
                {
                    sap.sa_data[trick] = rData[trick];
                }
                sap.sa_family = translateEnum(family);
                currentStateResult = bind(socket, &sap, endpoint.ip.rawLength());
            }

            if (currentStateResult == SOCKET_ERROR)
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
        if (currentStateResult == SOCKET_ERROR)
        {
            std::string err = "listen failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return;
        }
    }


    CrossSocket CrossSocket::acceptCS()
    {
        SOCKET socket = crsSocket;
        sockaddr_in  clientAddr4;
        sockaddr_in6 clientAddr6;
        int adrLength = 0;
        bool useV6 = false;
        //WARNING IPV4 use different structs than IPV6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        SOCKET client;
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
            std::string err = "accept failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return CrossSocket(err);
        }
        Endpoint endp;

        IpVersion clientIpVersion = IpVersion::IPv4;
        if (useV6)
        {
            clientIpVersion = IpVersion::IPv6;
            endp.ip.setBinaryData((char*)&(clientAddr6.sin6_addr.u.Byte), 16, clientIpVersion);
        }
        else
        {
            endp.ip.setBinaryData((char*)&(clientAddr4.sin_addr.S_un.S_addr), sizeof(clientAddr4.sin_addr.S_un), clientIpVersion);
        }
        
        endp.port = port;
        
        return CrossSocket(CSWL::ServerOrClient::CLIENT, port, family, type, protocol, endp, client);
    }

    int CrossSocket::connectCS()
    {
        return 0;
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
