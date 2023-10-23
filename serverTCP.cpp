#include "serverTCP.h"

// Constructor
serverTCP::serverTCP(int port, int newBacklog)
{
    backlog = newBacklog;
    memset(&serverAddr, 0, sizeof(serverAddr));
    // Code from SOCKETS-SERVER & CLIENT-2020 C++
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);
}

// Destructor
serverTCP::~serverTCP()
{
    // Close parent socket
    socClose(parentSocket);
    // Close all child sockets
    for(int i = 0; i < childSocket.size(); i++)
    {
        socClose(childSocket[i]);
    }
}

// Create socket
int serverTCP::socCreate()
{
    // Create and Check if create() failed
    if((parentSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Server socket Create() failed" << endl;
        exit(EXIT_FAILURE);
    }
    return parentSocket;
}

// Bind socket
void serverTCP::socBind(int socketNum)
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

// Listen for client
void serverTCP::socListen(int socketNum)
{
    // Start Listen() and Check if Listen failed
    if(listen(socketNum, backlog) < 0)
    {
        cout << "Server Listen() failed at socket " << socketNum << endl;
        // Close the socket in the case of failing
        socClose(socketNum);
        exit(EXIT_FAILURE);
    }
}

// Accept connection from client
int serverTCP::socAccept(int socketNum)
{
    socklen_t socketSize = sizeof(sockaddr_in);
    int newChildSocket = accept(socketNum, (struct sockaddr*) &clientAddr, &socketSize); // Code from SOCKETS-SERVER & CLIENT-2020 C++
    // Add into child socket vector
    if(newChildSocket > 0)
    {
        childSocket.push_back(newChildSocket);
    }
    return newChildSocket;
}

// Send packet to client
int serverTCP::socSend(int socketNum, string& message)
{
    int result = send(socketNum, message.c_str(), message.length(), 0);
    return result;
}

// Receive packet from client
int serverTCP::socReceive(int socketNum, char* buffer, int bufSize)
{
    memset(buffer, 0, bufSize);
    int result = recv(socketNum, buffer, bufSize, 0);
    return result;
} 

// Close/Destroy given socket
void serverTCP::socClose(int socketNum)
{
    close(socketNum);
}