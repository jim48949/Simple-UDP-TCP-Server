#include "serverUDP.h"
#include "clientUDP.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

/*
    Static Ports:
        1 UDP, 21000+xxx
*/

const int LAST_THREE_DIGIT = 797;
const int BUFFER_SIZE = 8192;
const string USERNAME_PASS = "100";
const string USERNAME_INVAILD = "101";
const string PWD_PASS = "110";
const string PWD_INVAILD = "111";
int usernameIndex;

string loginVaildation(string loginInfo, int mode);

int main(int arvc, char* argv[])
{
    // Assign port numbers
    int portUDP_C = 21000 + LAST_THREE_DIGIT;
    int portUDP_M = 24000 + LAST_THREE_DIGIT;
    // Assign buffer for messages
    char* bufferUDP_C = new char[BUFFER_SIZE];
    // Message to be sent / received
    string messageUDP_C;
    // Setup UDP Server / Client
    serverUDP* udpServerC = new serverUDP(portUDP_C);
    clientUDP* udpClientC = new clientUDP(portUDP_M);
    int udpSocketC = udpServerC->socCreate();
    udpServerC->socBind(udpSocketC);
    int udpSocketM = udpClientC->socCreate();

    cout << "The ServerC is up and running using UDP on port " << portUDP_C << endl;
    int counter = 1;
    while(true)
    {
        if(udpServerC->socReceive(udpSocketC, bufferUDP_C, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from UDP client " << udpSocketC << endl;
            exit(EXIT_FAILURE);
        }
        cout << "The ServerC received an authentication request from the Main Server." << endl;

        messageUDP_C = bufferUDP_C;
        // Check if username / pwd vaild
        string authRes = loginVaildation(messageUDP_C, counter);
        if(counter == 1)
        {
            // username
            counter++;
        }
        else if(counter == 2)
        {
            // pwd
            counter--;
        }
        // Authen passes, send confirmation
        if(udpClientC->socSend(udpSocketM, authRes) < 0)
        {
            cout << "Failed to send message to UDP server " << udpSocketM << endl;
            exit(EXIT_FAILURE);;
        }
        cout << "The ServerC finished sending the response to the Main Server." << endl;
    }

    // Release memory
    delete [] bufferUDP_C;
    delete udpServerC;
    delete udpClientC;

    return 0;
}


string loginVaildation(string loginInfo, int mode)
{
    vector<vector<string>> infos;
    string result = "NULL";
    ifstream infile;
    infile.open("cred.txt");

    string temp;
    vector<string> line;
    int counter = 0;
    // Parse all login infos into vector
    while(getline(infile, temp))
    {
        if(counter < 2)
        {
            // Same line
            if(temp[temp.length()-1] == '\n' || temp[temp.length()-1] == '\r')
            {
                temp = temp.substr(0,temp.length()-1);
            }
            line.push_back(temp);
            counter++;
        }
        
        if(counter == 2)
        {
            // Next line
            infos.push_back(line);
            line.clear();
            counter = 0;
        }
    }

    if(mode == 1)
    {
        for(int i = 0; i < infos.size(); i++)
        {
            
            // Compare username
            if(loginInfo.compare(infos[i][0]) == 0)
            {
                // username found & matched
                result = USERNAME_PASS;
                usernameIndex = i;
                break;
            }  
        }
    }
    else if(mode == 2)
    {
        // Compare password
        if(loginInfo.compare(infos[usernameIndex][1]) == 0)
        {
            // Password matched
            result = PWD_PASS;
        }
    }

    if(result.compare("NULL") == 0)
    {
        if(mode == 1)
        {
            // Username not found
            result = USERNAME_INVAILD;
        }
        else if(mode == 2)
        {
            // Password not matched
            result = PWD_INVAILD;
        }
    }

    infile.close();
    return result;
}