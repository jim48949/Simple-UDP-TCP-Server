#include "clientUDP.h"

// Constructor
clientUDP::clientUDP(int port)
{
    memset(&serverAddr, 0, sizeof(serverAddr));
    // Code from SOCKETS-SERVER & CLIENT-2020 C++
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);
}

// Destructor
clientUDP::~clientUDP()
{
    socClose(clientSocket);
}

// Create a new socket
int clientUDP::socCreate()
{
    // Create and check if failed
    if((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cout << "Client socket Create() failed" << endl;
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

// Send packet to Server
int clientUDP::socSend(int socketNum, string& message)
{
    int result = sendto(socketNum, message.c_str(), message.length(), 0, (struct sockaddr*) &serverAddr, sizeof(sockaddr_in));
    return result;
}

// Receive packet from Server
int clientUDP::socReceive(int socketNum, char* buffer, int bufSize)
{
    memset(buffer, 0, bufSize);
    socklen_t socketSize = sizeof(sockaddr_in);
    int result = recvfrom(socketNum, buffer, bufSize, 0, (struct sockaddr*) &serverAddr, &socketSize);
    return result;
}

// Close/Destroy client socket
void clientUDP::socClose(int socketNum)
{
    close(socketNum);
}