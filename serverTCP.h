#pragma once
#include <netinet/in.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

class serverTCP
{
    private:
        vector<int> childSocket; // For TCP clients
        int parentSocket; // For TCP server
        int backlog; // Maximum number of queued connections, always >= 0
        sockaddr_in serverAddr; // Server's socket address
        sockaddr_in clientAddr; // Clients' socket address

    public:
        serverTCP(int port, int newBacklog); // Constructor
        ~serverTCP(); // Destructor
        int socCreate(); // Create socket
        void socBind(int socketNum); // Bind socket
        void socListen(int socketNum); // Listen for client
        int socAccept(int socketNum); // Accept connect from client
        int socSend(int socketNum, string& message); // Send packet to client
        int socReceive(int socketNum, char* buffer, int bufSize); // Receive packet from client
        void socClose(int socketNum); // Close/destroy given socket

};
