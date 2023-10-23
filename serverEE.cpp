#include "serverUDP.h"
#include "clientUDP.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

/*
    Static Ports:
        1 UDP, 23000+xxx
*/

const int LAST_THREE_DIGIT = 797;
const int BUFFER_SIZE = 8192;

string getCourseInfo(string courseCode, string category);

int main(int argc, char* argv[])
{
    // Assign port numbers
    int portUDP_EE = 23000 + LAST_THREE_DIGIT;
    int portUDP_M = 24000 + LAST_THREE_DIGIT;
    // Assign buffer for messages
    char* bufferUDP_EE = new char[BUFFER_SIZE];
    // Message to be sent / received
    string messageUDP_EE;
    // Setup UDP Server / Client
    serverUDP* udpServerEE = new serverUDP(portUDP_EE);
    clientUDP* udpClientEE = new clientUDP(portUDP_M);
    int udpSocketEE = udpServerEE->socCreate();
    udpServerEE->socBind(udpSocketEE);
    int udpSocketM = udpClientEE->socCreate();
    // Course Info Vars
    string category;
    string courseCode;

    cout << "The ServerEE is up and running using UDP on port " << portUDP_EE << endl;
    int counter = 1;
    
    while(true)
    {
        if(udpServerEE->socReceive(udpSocketEE, bufferUDP_EE, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from UDP client " << udpSocketEE << endl;
            exit(EXIT_FAILURE);
        }
        // Counter to determine which message came in / info needed
        if(counter == 1)
        {
            courseCode = bufferUDP_EE;
            counter++;
        }
        else if(counter == 2)
        {
            category = bufferUDP_EE;
            cout << "The ServerEE received a request from the Main Server about the "
                 << category << " of " << courseCode << "." << endl;

            messageUDP_EE = getCourseInfo(courseCode, category);
            if(messageUDP_EE.compare("NULL") == 0)
            {
                cout << "Didn’t find the course: " << courseCode << endl;
            }
            else
            {
                cout << "The course information has been found: The " 
                     << category << " of " << courseCode << " is " << messageUDP_EE << endl;
            }
            // Send back the requested course info
            if(udpClientEE->socSend(udpSocketM, messageUDP_EE) < 0)
            {
                cout << "Failed to send message to UDP server " << udpSocketM << endl;
                exit(EXIT_FAILURE);
            }
            cout << "The ServerEE finished sending the response to the Main Server." << endl;
 
            // Reset counter
            counter--;
        }
        else
        {
            cout << "Failed: serverEE counter invaild" << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Release memory
    delete [] bufferUDP_EE;
    delete udpClientEE;
    delete udpServerEE;

    return 0;
}

int getCategoryIndex(string category)
{
    int index = -1;

    if(category.compare("Credit") == 0)
    {
        index = 1;
    }
    else if(category.compare("Professor") == 0)
    {
        index = 2;
    }
    else if(category.compare("Days") == 0)
    {
        index = 3;
    }
    else if(category.compare("CourseName") == 0)
    {
        index = 4;
    }

    return index;
}

string getCourseInfo(string courseCode, string category)
{
    vector<vector<string>> allInfos;
    string info = "";
    ifstream infile;
    infile.open("ee.txt");
    string temp;
    vector<string> line;
    // Get all course Info
    while(getline(infile, temp))
    {
        stringstream wordStream(temp);
        while(wordStream.good())
        {
            string thisWord;
            getline(wordStream, thisWord, ',');
            line.push_back(thisWord);
        }
        allInfos.push_back(line);
        line.clear();
    }

    // Get specified course info
    for(int i = 0; i < allInfos.size(); i++)
    {
        // Find correct course
        if(allInfos[i][0].compare(courseCode) == 0)
        {
            if(category.compare("all") != 0)
            {
                // Find specified category
                int ind = getCategoryIndex(category);
                if(ind > 0)
                {
                    info = allInfos[i][ind]; 
                }              
            }
            else
            {
                // Get all categories
                for(int j = 1; j < allInfos[i].size(); j++)
                {
                    if(j == allInfos[i].size() - 1)
                    {
                        info += allInfos[i][j];
                    }
                    else
                    {
                        info += allInfos[i][j] + ", ";
                    }
                }
            }   
            break;
        }
    }

    if(info.compare("") == 0)
    {
        info = "NULL";
    }

    infile.close();
    return info;
}