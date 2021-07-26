

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
        return AF_BTH;
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

void CrossSocket::createSocket(std::string domainOrIp)
{
    

    if (this->behaviour == CSWL::ServerOrClient::SERVER)
    {
        //Server
        CSWL::Endpoint entp = CSWL::Resolver::resolveLocalServerAddress(this->port, *this);
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

    }
    else if(domainOrIp.length() > 0)
    {
        //Client
        CSWL::Endpoint entp = CSWL::Resolver::resolveAddress(domainOrIp, this->port, *this);
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
    }
    else
    {
        std::string err = "tried to create a clientsocket without or empty aim (domain / dotted ip).";
        setError(err);
    }
}

int CrossSocket::connect()
{
    return 0;
}

int CrossSocket::bind()
{
    return 0;

}
int CrossSocket::listen()
{
    return 0;

}
int CrossSocket::accept()
{
    return 0;

}

int CrossSocket::receive()
{
    return 0;

}

int CrossSocket::send()
{
    return 0;

}

int CrossSocket::shutdown()
{
    return 0;

}



