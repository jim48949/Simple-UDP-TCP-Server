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

class clientTCP
{
    private:
        int socClient;
        sockaddr_in clientAddr;
    public:
        clientTCP(int port); // Constructor
        ~clientTCP(); // Destructor
        int socCreate(); // Create new socket for client
        void socConnect(int socketNum); // Establish connection to server
        int socSend(int socketNum, string& message); // Send packet to server
        int socReceive(int socketNum, char* buffer, int bufSize); // Receive packet from server
        void socClose(int socketNum); // Close/Destroy client socket   
};
