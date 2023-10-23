#include "serverUDP.h"
#include "clientUDP.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

/*
    Static Ports:
        1 UDP, 22000+xxx
*/

const int LAST_THREE_DIGIT = 797;
const int BUFFER_SIZE = 8192;

string getCourseInfo(string courseCode, string category);

int main(int argc, char* argv[])
{
    // Assign port numbers
    int portUDP_CS = 22000 + LAST_THREE_DIGIT;
    int portUDP_M = 24000 + LAST_THREE_DIGIT;
    // Assign buffer for messages
    char* bufferUDP_CS = new char[BUFFER_SIZE];
    // Message to be sent / received
    string messageUDP_CS;
    // Setup UDP Server / Client
    serverUDP* udpServerCS = new serverUDP(portUDP_CS);
    clientUDP* udpClientCS = new clientUDP(portUDP_M);
    int udpSocketCS = udpServerCS->socCreate();
    udpServerCS->socBind(udpSocketCS);
    int udpSocketM = udpClientCS->socCreate();
    // Course Info Vars
    string category;
    string courseCode;

    cout << "The ServerCS is up and running using UDP on port " << portUDP_CS << endl;
    int counter = 1;
    
    while(true)
    {
        if(udpServerCS->socReceive(udpSocketCS, bufferUDP_CS, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from UDP client " << udpSocketCS << endl;
            exit(EXIT_FAILURE);
        }
        
        // Counter to determine which message came in / info needed
        if(counter == 1)
        {
            courseCode = bufferUDP_CS;
            counter++;
        }
        else if(counter == 2)
        {
            category = bufferUDP_CS;
            cout << "The ServerCS received a request from the Main Server about the "
                 << category << " of " << courseCode << "." << endl;

            messageUDP_CS = getCourseInfo(courseCode, category);
            if(messageUDP_CS.compare("NULL") == 0)
            {
                cout << "Didn’t find the course: " << courseCode << endl;
            }
            else
            {
                cout << "The course information has been found: The " 
                     << category << " of " << courseCode << " is " << messageUDP_CS << endl;
            }
            // Send back the requested course info
            if(udpClientCS->socSend(udpSocketM, messageUDP_CS) < 0)
            {
                cout << "Failed to send message to UDP server " << udpSocketM << endl;
                exit(EXIT_FAILURE);
            }
            cout << "The ServerCS finished sending the response to the Main Server." << endl;

            // Reset counter
            counter--;
        }
        else
        {
            cout << "Failed: serverCS counter invaild" << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Release memory
    delete [] bufferUDP_CS;
    delete udpClientCS;
    delete udpServerCS;

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
    infile.open("cs.txt");
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