#include "clientTCP.h"

// Constructor
clientTCP::clientTCP(int port)
{
    memset(&clientAddr, 0, sizeof(clientAddr));
    // Code from SOCKETS-SERVER & CLIENT-2020 C++
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(port);
}

// Destructor
clientTCP::~clientTCP()
{
    // Close & destroy client socket
    socClose(socClient);
}

// Create new socket for client
int clientTCP::socCreate()
{
    // Create and check if failed
    if((socClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Client socket Create() failed" << endl;
        exit(EXIT_FAILURE);
    }
    return socClient;
}

// Establish connection to socket
void clientTCP::socConnect(int socketNum)
{
    if(connect(socketNum, (struct sockaddr*) &clientAddr, sizeof(clientAddr)) < 0)
    {
        cout << "Client Connect() failed at socket " << socketNum << endl;
        // Close the socket in the case of failing
        socClose(socketNum);
        exit(EXIT_FAILURE);
    }
}

// Send packet to server
int clientTCP::socSend(int socketNum, string& message)
{
    int result = send(socketNum, message.c_str(), message.length(), 0);
    return result;
}

// Receive packet from server
int clientTCP::socReceive(int socketNum, char* buffer, int bufSize) 
{
    memset(buffer, 0, bufSize);
    int result = recv(socketNum, buffer, bufSize, 0);
    return result;
}

// Close/Destroy client socket
void clientTCP::socClose(int socketNum)
{
    close(socketNum);
}  