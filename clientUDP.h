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

class clientUDP
{
    private:
        int clientSocket;
        sockaddr_in serverAddr;
    public:
        clientUDP(int port); // Constructor
        ~clientUDP(); // Destructor
        int socCreate(); // Create a new socket
        int socSend(int socketNum, string& message); // Send packet to client
        int socReceive(int socketNum, char* buffer, int bufSize); // Receive packet from client
        void socClose(int socketNum); // Close/Destroy server socket
};
