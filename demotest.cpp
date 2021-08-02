#include "CrossSocket.h"
#include <stdio.h>

enum class CSWLDEMOMODE
{
	SERVERIPV4, CLIENTIPV4
};

int main()
{
	CSWLDEMOMODE MODE = CSWLDEMOMODE::CLIENTIPV4;
	
	if (MODE == CSWLDEMOMODE::SERVERIPV4)
	{
		CSWL::IpAddress test6;
		test6.setFormatedData("2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
		if (test6.toString() != "2001:0db8:85a3:08d3:1319:8a2e:0370:7344")
		{
			printf("Error in class IpAddress toString() has wrong format!");
		}

		CSWL::SocketType socketType = CSWL::SocketType::CS_SOCK_STREAM;

		CSWL::CrossSocket socket = CSWL::CrossSocket(CSWL::ServerOrClient::SERVER, 80, CSWL::AddressFamily::CS_AF_INET, socketType, CSWL::IpProtocol::CS_IPPROTO_TCP, 333);
		if (!socket.actionSuccess())
		{
			printf("Error creating Socket: %s\n", socket.lastError().c_str());
		}
		
		CSWL::Endpoint endpoint = socket.getEntpoint();
		printf("Endpoint of My socket: %s\n", endpoint.toString().c_str());


		CSWL::CrossSocket client = socket;
		if (socketType == CSWL::SocketType::CS_SOCK_STREAM)
		{
			CSWL::CrossSocket client = socket.acceptCS();
			if (!socket.actionSuccess())
			{
				printf("Error accepting client Socket: %s\n", socket.lastError().c_str());
			}
			printf("Endpoint of Client socket: %s\n", client.getEntpoint().toString().c_str());
		}

		unsigned char receiveBuffer[1024];
		size_t totalReceived = 0;

		std::string message = "It works! This is a test application.";
		std::string miniHTML = "<html><head><title>demotest.cpp</title></head><body><h1>" + message + "</h1></body></html>";

		std::string sendMinimalHTTPresponse = "HTTP/2 200 OK\r\ncontent-type: text/html\r\ncontent-length: " + std::to_string(miniHTML.size()) + "\r\n\r\n";
		sendMinimalHTTPresponse += miniHTML;

		std::string storage;
		int received = 0;
		bool isUDP = socketType == CSWL::SocketType::CS_SOCK_DGRAM;
		bool udpIsConnected = false;

		do
		{
			received = client.receiveCS(receiveBuffer, 1024);
			if (!client.actionSuccess())
			{
				if (isUDP && !udpIsConnected)
				{
					if (client.errorIsTimeout())
					{
						continue;
					}
				}
				printf("Receive error: %s", client.lastError().c_str());
				break;
			}
			udpIsConnected = true;
			totalReceived += received;

			//Be careful of using this strategy :D
			storage.append((const char*)receiveBuffer, received);
		} while (received > 0);

		size_t totalSent = 0;
		while (totalSent < sendMinimalHTTPresponse.size())
		{
			totalSent += client.sendCS((const unsigned char*)(sendMinimalHTTPresponse.c_str() + totalSent), sendMinimalHTTPresponse.size() - totalSent);
		}
	}
	else if (MODE == CSWLDEMOMODE::CLIENTIPV4)
	{

		CSWL::SocketType socketType = CSWL::SocketType::CS_SOCK_STREAM;
		CSWL::CrossSocket socket = CSWL::CrossSocket(CSWL::ServerOrClient::CLIENT, 80, CSWL::AddressFamily::CS_AF_INET, socketType, CSWL::IpProtocol::CS_IPPROTO_TCP);
		if (!socket.actionSuccess())
		{
			printf("Error creating Socket: %s\n", socket.lastError().c_str());
		}

		CSWL::Endpoint endpoint = socket.getEntpoint();
		printf("Endpoint of My socket: %s\n", endpoint.toString().c_str());
		std::string host = "www.http2demo.io";
		if (socketType == CSWL::SocketType::CS_SOCK_STREAM)
		{
			if (!socket.connectCS(host))
			{
				printf("Error connecting client Socket: %s\n", socket.lastError().c_str());
				return 0;
			}
		}

		printf("Endpoint of Server socket: %s\n", socket.getEntpoint().toString().c_str());

		unsigned char receiveBuffer[1024];
		size_t totalReceived = 0;

		std::string sendMinimalHTTPrequest = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n ";

		size_t totalSent = 0;
		while (totalSent < sendMinimalHTTPrequest.size())
		{
			totalSent += socket.sendCS((const unsigned char*)(sendMinimalHTTPrequest.c_str() + totalSent), sendMinimalHTTPrequest.size() - totalSent);
		}

		std::string storage;
		int received = 0;
		bool isUDP = socketType == CSWL::SocketType::CS_SOCK_DGRAM;
		bool udpIsConnected = false;
		do
		{
			received = socket.receiveCS(receiveBuffer, 1024);
			if (!socket.actionSuccess())
			{
				if (isUDP && !udpIsConnected)
				{
					if (socket.errorIsTimeout())
					{
						continue;
					}
				}
				printf("Receive error: %s", socket.lastError().c_str());
				break;
			}
			udpIsConnected = true;
			totalReceived += received;

			//Be careful of using this strategy :D
			storage.append((const char*)receiveBuffer, received);
		} while (received > 0);

		printf("OUTPUT:\n\n");

		for (int i = 0; i < storage.size(); i+=500)
		{
			if (i + 500 < storage.size())
			{
				printf("%s", storage.substr(i, 500).c_str());
			}
			else
			{
				printf("%s", storage.substr(i, storage.size()-i).c_str());
				break;
			}
		}

	}

	return 0;
}









