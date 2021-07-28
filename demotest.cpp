#include "CrossSocket.h"
#include <stdio.h>


int main()
{

	CSWL::IpAddress test6;
	test6.setFormatedData("2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
	if (test6.toString() != "2001:0db8:85a3:08d3:1319:8a2e:0370:7344")
	{
		printf("Error in class IpAddress toString has wrong format!");
	}

	CrossSocket socket = CrossSocket(CSWL::ServerOrClient::SERVER, 8080, CSWL::AddressFamily::CS_AF_INET, CSWL::SocketType::CS_SOCK_STREAM, CSWL::IpProtocol::CS_IPPROTO_TCP);
	if (!socket.actionSuccess())
	{
		printf("Error creating Socket: %s", socket.lastError().c_str());
	}

	CSWL::Endpoint endpoint = socket.getEntpoint();
	printf("Endpoint of socket: [%s]:%s", endpoint.ip.toString().c_str(), std::to_string(endpoint.port).c_str());
	
	
	return 0;
}









