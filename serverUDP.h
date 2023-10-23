#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class serverUDP
{
    private:
        int serverSocket;
        sockaddr_in serverAddr;
        sockaddr_in clientAddr;
    public: 
        serverUDP(int port); // Constructor
        ~serverUDP(); // Destructor
        int socCreate(); // Create a new socket
        void socBind(int socketNum); // Bind the socket
        int socSend(int socketNum, string& message); // Send packet to client
        int socReceive(int socketNum, char* buffer, int bufSize); // Receive packet from client
        void socClose(int socketNum); // Close/Destroy server socket
};
