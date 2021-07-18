#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



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

void CrossSocket::createSocket()
{
   SOCKET sock = socket(translateEnum(this->family), translateEnum(this->type), translateEnum(this->protocol));
   crsSocket = sock;

}

int CrossSocket::connect()
{

}

int CrossSocket::bind()
{


}
int CrossSocket::listen()
{


}
int CrossSocket::accept()
{


}

int CrossSocket::receive()
{


}

int CrossSocket::send()
{


}

int CrossSocket::shutdown()
{


}



