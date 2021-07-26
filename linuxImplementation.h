

inline void CrossSocket::initialisation()
{
	//Not needed
}

void CrossSocket::deconstruction()
{
   
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





