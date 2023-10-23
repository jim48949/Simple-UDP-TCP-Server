#include "serverUDP.h"

// Constructor
serverUDP::serverUDP(int port)
{
    memset(&serverAddr, 0, sizeof(serverAddr));
    // Code from SOCKETS-SERVER & CLIENT-2020 C++
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);
}

// Destructor
serverUDP::~serverUDP()
{
    socClose(serverSocket);
}

// Create a new socket
int serverUDP::socCreate()
{
    // Create and check if failed
    if((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cout << "Server socket Create() failed" << endl;
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

// Bind the socket
void serverUDP::socBind(int socketNum)
{
    // Bind and Check if bind failed
    if(bind(socketNum, (struct sockaddr*) &serverAddr, sizeof(sockaddr_in)) < 0) // Code from SOCKETS-SERVER & CLIENT-2020 C++
    {
        cout << "Server Bind() failed at socket " << socketNum << endl;
        // Close the socket in the case of failing
        socClose(socketNum);
        exit(EXIT_FAILURE);
    }
}

// Send packet to client
int serverUDP::socSend(int socketNum, string& message)
{
    int result = sendto(socketNum, message.c_str(), message.length(), 0, (struct sockaddr*) &clientAddr, sizeof(sockaddr_in));
    return result;
}

// Receive packet from client
int serverUDP::socReceive(int socketNum, char* buffer, int bufSize)
{
    memset(buffer, 0, bufSize);
    socklen_t socketSize = sizeof(sockaddr_in);
    int result = recvfrom(socketNum, buffer, bufSize, 0, (struct sockaddr*) &clientAddr, &socketSize);
    return result;
}

// Close/Destroy server socket
void serverUDP::socClose(int socketNum)
{
    close(socketNum);
}