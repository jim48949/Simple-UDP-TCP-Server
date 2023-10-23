#include "serverTCP.h"
#include "serverUDP.h"
#include "clientUDP.h"
#include <iostream>

using namespace std;

/*
    Static Ports:
        1 UDP, 24000+xxx
        1 TCP, 25000+xxx
*/

const int LAST_THREE_DIGIT = 797;
const int BUFFER_SIZE = 8192;
const string USERNAME_PASS = "100";
const string USERNAME_INVAILD = "101";
const string PWD_PASS = "110";
const string PWD_INVAILD = "111";

string encrypInfo(string info);

int main(int argc, char* argv[])
{
    // Assign port numbers
    int portTCP = 25000 + LAST_THREE_DIGIT;
    int portUDP = 24000 + LAST_THREE_DIGIT;
    // Port numbers where the server will be receiving message from
    int portUDP_C = 21000 + LAST_THREE_DIGIT;
    int portUDP_CS = 22000 + LAST_THREE_DIGIT;
    int portUDP_EE = 23000 + LAST_THREE_DIGIT;
    // Assign buffer for messages
    char* bufferTCP = new char[BUFFER_SIZE];
    char* bufferUDP = new char[BUFFER_SIZE];
    char* bufferUDPC = new char[BUFFER_SIZE];
    char* bufferUDPCS = new char[BUFFER_SIZE];
    char* bufferUDPEE = new char[BUFFER_SIZE];
    // Message to be sent / received
    string messageTCP = "Server test message";
    string messageUDP;
    string messageUDPC, messageUDPCS, messageUDPEE;
    // Username
    string username;

    // Setting up Servers
    
    // Setup TCP Server
    // Only one clients will be allowed to connect at a time
    serverTCP* tcpServer = new serverTCP(portTCP, 1);
    int tcpSocket = tcpServer->socCreate();
    tcpServer->socBind(tcpSocket);
    tcpServer->socListen(tcpSocket);

    // Setup UDP Server
    serverUDP* udpServer = new serverUDP(portUDP);
    int udpSocket = udpServer->socCreate();
    udpServer->socBind(udpSocket);
    // Setup UDP Clients
    clientUDP* udpClientC = new clientUDP(portUDP_C);
    clientUDP* udpClientCS = new clientUDP(portUDP_CS);
    clientUDP* udpClientEE = new clientUDP(portUDP_EE);
    int udpSocketC = udpClientC->socCreate();
    int udpSocketCS = udpClientCS->socCreate();
    int udpSocketEE = udpClientEE->socCreate();

    cout << "The main server is up and running." << endl;
    // Accept connect request from tcp client
    int tcpChildSocket = tcpServer->socAccept(tcpSocket); // Client socket
    if(tcpChildSocket < 0)
    {
        cout << "Failed to accept TCP Socket from client!" << tcpChildSocket << endl;
        exit(EXIT_FAILURE);
    }

    // Authentication part
    int loginVaild = 0;
    while(loginVaild < 2)
    { 
        // Receive message from tcp client
        if(tcpServer->socReceive(tcpChildSocket, bufferTCP, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from TCP client " << tcpChildSocket << endl;
            exit(EXIT_FAILURE);
        }
        messageTCP = bufferTCP;
        cout << "The main server received the authentication for " << messageTCP << " using TCP over port " << portTCP << endl;

        // Encryption part
        messageUDPC = encrypInfo(messageTCP);
        // Send user name to server C
        if(udpClientC->socSend(udpSocketC, messageUDPC) < 0)
        {
            cout << "Failed to send message to UDP server " << udpSocketC << endl;
            exit(EXIT_FAILURE);
        }
        cout << "The main server sent an authentication request to serverC." << endl;
        // Receive response from server C
        if(udpServer->socReceive(udpSocket, bufferUDP, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from UDP client " << udpSocket << endl;
            exit(EXIT_FAILURE);
        }
        cout << "The main server received the result of the authentication request from ServerC using UDP over port " << portUDP_C << endl;
        string authRes = bufferUDP;
        // Send result back to client
        if(tcpServer->socSend(tcpChildSocket, authRes) < 0)
        {
            cout << "Failed to send message to TCP client " << tcpChildSocket << endl;
            exit(EXIT_FAILURE);
        }
        cout << "The main server sent the authentication result to the client." << endl << flush;

        // Check if Authentication passes
        if(authRes.compare(USERNAME_PASS) == 0)
        {
            // username vaild
            loginVaild++;
            username = messageTCP; // Store username
        }
        else if(authRes.compare(PWD_PASS) == 0)
        {
            // Pwd vaild
            loginVaild++;
        }
        else 
        {
            // username / pwd invaild
            loginVaild = 0;
        }
    }

    // Query part
    int queryOrder = 1; // 1 for course code, 2 for category
    string courseCode, category;
    while(true)
    {
        // Receive message from tcp client
        if(tcpServer->socReceive(tcpChildSocket, bufferTCP, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from TCP client " << tcpChildSocket << endl;
            exit(EXIT_FAILURE); 
        }
        if(queryOrder == 1)
        {
            courseCode = bufferTCP;
            queryOrder++;
        }
        else if(queryOrder == 2)
        {
            category = bufferTCP;
            cout << "The main server received from " << username 
                << " to query course " << courseCode << " about " << category
                << " using TCP over port " << portTCP << endl;
            
            // Determine which server to send request
            if((courseCode.substr(0,2)).compare("CS") == 0)
            {
                // Send request to CS server
                // First send course code
                if(udpClientCS->socSend(udpSocketCS, courseCode) < 0)
                {
                    cout << "Failed to send message to UDP server " << udpSocketCS << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent a request to serverCS." << endl;
                // Send category
                if(udpClientCS->socSend(udpSocketCS, category) < 0)
                {
                    cout << "Failed to send message to UDP server " << udpSocketCS << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent a request to serverCS." << endl;
                
                // Receive info from CS server
                if(udpServer->socReceive(udpSocket, bufferUDP, BUFFER_SIZE) < 0)
                {
                    cout << "Failed to receive message from UDP client " << udpSocket << endl;
                    exit(EXIT_FAILURE); 
                }
                cout << "The main server received the response from serverCS using UDP over port " << portUDP << "." << endl;
                
                // Send the query info back to client
                messageTCP = bufferUDP;
                if(tcpServer->socSend(tcpChildSocket, messageTCP) < 0)
                {
                    cout << "Failed to send message to TCP client " << tcpChildSocket << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent the query information to the client." << endl;
            }
            else if((courseCode.substr(0,2)).compare("EE") == 0)
            {
                // Send request to EE server
                // First send course code
                if(udpClientEE->socSend(udpSocketEE, courseCode) < 0)
                {
                    cout << "Failed to send message to UDP server " << udpSocketEE << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent a request to serverEE." << endl;
                // Send category
                if(udpClientEE->socSend(udpSocketEE, category) < 0)
                {
                    cout << "Failed to send message to UDP server " << udpSocketEE << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent a request to serverEE." << endl;
                
                // Receive info from EE server
                if(udpServer->socReceive(udpSocket, bufferUDP, BUFFER_SIZE) < 0)
                {
                    cout << "Failed to receive message from UDP client " << udpSocket << endl;
                    exit(EXIT_FAILURE); 
                }
                cout << "The main server received the response from serverEE using UDP over port " << portUDP << "." << endl;
                
                // Send the query info back to client
                messageTCP = bufferUDP;
                if(tcpServer->socSend(tcpChildSocket, messageTCP) < 0)
                {
                    cout << "Failed to send message to TCP client " << tcpChildSocket << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "The main server sent the query information to the client." << endl;
            }
            else
            {
                cout << "Course Department does not exist!" << endl;
            }

            queryOrder--;
        }
        
    }

    // Release memory
    delete [] bufferTCP;
    delete [] bufferUDP;
    delete [] bufferUDPC;
    delete [] bufferUDPCS;
    delete [] bufferUDPEE;
    delete tcpServer;
    delete udpServer;
    delete udpClientC;
    delete udpClientCS;
    delete udpClientEE;

    return 0;
}


string encrypInfo(string info)
{
    string encrypedInfo = "";
    // Start encryption
    for(int i = 0; i < info.length(); i++)
    {
        char curChar = info[i];
        char encrypedChar;
        
        if(curChar >= '0' && curChar <= '9') // Check if numbers
        {  
            if(curChar >= '6' && curChar <= '9')
            {
                // Check if special case (6-9)
                encrypedChar = curChar - '6' + '0';
            }
            else
            {
                // Normal case (0-5)
                encrypedChar = char(curChar + 4);
            }
        }
        else if(curChar >= 'A' && curChar <= 'Z') // Check if upper case letter
        {
            if(curChar >= 'W' && curChar <= 'Z')
            {
                // Check if special case (W-Z)
                encrypedChar = curChar - 'W' + 'A';
            }
            else
            {
                // Normal case (A-V)
                encrypedChar = char(curChar + 4);
            }
        }
        else if(curChar >= 'a' && curChar <= 'z') // Check if lower case letter
        {
            if(curChar >= 'w' && curChar <= 'z')
            {
                // Check if special case (w-z)
                encrypedChar = curChar - 'w' + 'a';
            }
            else
            {
                // Normal case (a-v)
                encrypedChar = char(curChar + 4);
            }
        }
        else // Special letter
        {   
            // Do nothing
            encrypedChar = curChar;
        }
        encrypedInfo += encrypedChar;
    }
    
    return encrypedInfo;
}