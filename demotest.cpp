#include "CrossSocket.h"
#include <stdio.h>


int main()
{

	CSWL::IpAddress test6;
	test6.setFormatedData("2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
	if (test6.toString() != "2001:0db8:85a3:08d3:1319:8a2e:0370:7344")
	{
		printf("Error in class IpAddress toString() has wrong format!");
	}

	CSWL::CrossSocket socket = CSWL::CrossSocket(CSWL::ServerOrClient::SERVER, 8080, CSWL::AddressFamily::CS_AF_INET, CSWL::SocketType::CS_SOCK_STREAM, CSWL::IpProtocol::CS_IPPROTO_TCP);
	if (!socket.actionSuccess())
	{
		printf("Error creating Socket: %s\n", socket.lastError().c_str());
	}

	CSWL::Endpoint endpoint = socket.getEntpoint();
	printf("Endpoint of My socket: %s\n", endpoint.toString().c_str());

	CSWL::CrossSocket client = socket.acceptCS();
	if (!socket.actionSuccess())
	{
		printf("Error accepting client Socket: %s\n", socket.lastError().c_str());
	}


	printf("Endpoint of Client socket: %s\n", client.getEntpoint().toString().c_str());



	
	return 0;
}









