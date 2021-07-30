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

	CSWL::CrossSocket socket = CSWL::CrossSocket(CSWL::ServerOrClient::SERVER, 80, CSWL::AddressFamily::CS_AF_INET, CSWL::SocketType::CS_SOCK_STREAM, CSWL::IpProtocol::CS_IPPROTO_TCP);
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

	int size = 0;

	unsigned char receiveBuffer[1024];
	size_t totalReceived = 0;

	std::string message = "It works! This is a test application.";
	std::string miniHTML = "<html><head><title>demotest.cpp</title></head><body><h1>" + message + "</h1></body></html>";

	std::string sendMinimalHTTPresponse = "HTTP/2 200 OK\r\ncontent-type: text/html\r\ncontent-length: " + std::to_string(miniHTML.size()) + "\r\n\r\n";
	sendMinimalHTTPresponse += miniHTML;

	std::string storage;
	do
	{
		int received = client.receiveCS(receiveBuffer, 1024);
		if (!client.actionSuccess())
		{
			printf("Receive error: %s", client.lastError().c_str());
			break;
		}
		totalReceived += received;

		//Be careful of using this strategy :D
		storage.append((const char*)receiveBuffer, received);
	} while (size > 0);

	size_t totalSent = 0;
	while (totalSent < sendMinimalHTTPresponse.size())
	{
		totalSent += client.sendCS((const unsigned char*)(sendMinimalHTTPresponse.c_str() + totalSent), sendMinimalHTTPresponse.size() - totalSent);
	}
	return 0;
}









