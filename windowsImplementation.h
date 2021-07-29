
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
            Endpoint entp = Resolver::resolveLocalServerAddress(this->port, *this);
            if (!this->actionSuccess())
            {
                //Error was seted by resolver.
                return;
            }
            endpoint = entp;

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
            //Linker LNK2019 linking error unresolved extern symbol
            std::vector<Endpoint> entps = Resolver::resolveAddresses(domainOrIp, this->port, *this);
            if (!this->actionSuccess())
            {
                //Error was seted by resolver.
                return;
            }
            endpoint = entps[0];


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
        SOCKET socket = crsSocket;
        sockaddr sap;
        char* rData = endpoint.ip.rawData();
        for (int trick = 0; trick < 14; trick++)
        {
            sap.sa_data[trick] = rData[trick];
        }
        sap.sa_family = translateEnum(family);
        currentStateResult = bind(socket, &sap, endpoint.ip.rawLength());
        if (currentStateResult == SOCKET_ERROR)
        {
            std::string err = "bind failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return;
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

        sockaddr clientAddr;
        int adrLength = 0;
        SOCKET client = accept(socket, &clientAddr, &adrLength);
        if (client == INVALID_SOCKET)
        {
            std::string err = "accept failed with error: ";
            err += std::to_string(WSAGetLastError());
            setError(err);
            return CrossSocket(err);
        }
        Endpoint endp;

        IpVersion clientIpVersion = IpVersion::IPv4;

        if (clientAddr.sa_family == translateEnum(AddressFamily::CS_AF_INET6))
        {
            clientIpVersion = IpVersion::IPv6;
        }

        endp.ip.setBinaryData(clientAddr.sa_data, adrLength, clientIpVersion);
        endp.port = port;
        
        return CrossSocket(CSWL::ServerOrClient::CLIENT, port, family, type, protocol, endp);
    }

    int CrossSocket::connectCS()
    {
        return 0;
    }

    int CrossSocket::receiveCS()
    {
        return 0;

    }

    int CrossSocket::sendCS()
    {
        return 0;

    }

    int CrossSocket::shutdownCS()
    {
        return 0;

    }


}
