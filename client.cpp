#include "clientTCP.h" 
#include <iostream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

const int LAST_THREE_DIGIT = 797;
const int BUFFER_SIZE = 8192;
const string USERNAME_PASS = "100";
const string USERNAME_INVAILD = "101";
const string PWD_PASS = "110";
const string PWD_INVAILD = "111";

int main(int argc, char* argv[])
{
    // Assign port number (server port)
    int portTCP = 25000 + LAST_THREE_DIGIT;
    // Message holder & Buffer
    char* bufferTCP = new char[BUFFER_SIZE];
    string messageTCP = "Test message";
    // Set up TCP client
    clientTCP* tcpClient = new clientTCP(portTCP);
    int clientScoket = tcpClient->socCreate();
    tcpClient->socConnect(clientScoket);
    // Query Variables
    string courseCode;
    string category;
    string allCourses = "";
    vector<string> courses;

    cout << "The client is up and running." << endl;

    // Prompt user to enter username and pwd
    string username;
    string pwd;
    int attempts = 3;
    while(attempts != 0)
    {
        cout << "Please enter the username:";
        cin >> username;
        cout << "Please enter the password:";
        cin >> pwd;

        // Send username and check if failed
        if(tcpClient->socSend(clientScoket, username) < 0)
        {
            cout << "Failed to send message from socket " << clientScoket << endl;
            exit(EXIT_FAILURE);
        }
        cout << username << " sent an suthentication request to the main server" << endl;
        // Receive message from main server
        if(tcpClient->socReceive(clientScoket, bufferTCP, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from socket " << clientScoket << endl;
            exit(EXIT_FAILURE);
        }
        string authRes1 = bufferTCP;

        // Send pwd and check if failed
        if(tcpClient->socSend(clientScoket, pwd) < 0)
        {
            cout << "Failed to send message from socket " << clientScoket << endl;
            exit(EXIT_FAILURE);
        }
        cout << username << " sent an suthentication request to the main server" << endl;
        // Receive message from main server
        if(tcpClient->socReceive(clientScoket, bufferTCP, BUFFER_SIZE) < 0)
        {
            cout << "Failed to receive message from socket " << clientScoket << endl;
            exit(EXIT_FAILURE);
        }
        string authRes2 = bufferTCP;

        // Check authentication result
        if(authRes1.compare(USERNAME_PASS) != 0 || authRes2.compare(PWD_PASS) != 0)
        {
            attempts--;
            if(authRes1.compare(USERNAME_INVAILD) == 0)
            {
                cout << username << " received the result of authentication using TCP over port " << portTCP <<". "
                << "Authentication failed: Username Does not exist" << endl
                << "Attempts remaining: " << attempts << endl;
            }
            else if(authRes2.compare(PWD_INVAILD) == 0)
            {
                cout << username << " received the result of authentication using TCP over port " << portTCP <<". "
                << "Authentication failed: Password does not match" << endl
                << "Attempts remaining: " << attempts << endl;
            }
        }
        else
        {
            cout << username << " received the result of authentication using TCP over port " << portTCP <<". "
                    << "Authentication is successful" << endl;
            break;
        }
    }
    
    if(attempts == 0)
    {
        cout << "Authentication Failed for 3 attempts. Client will shut down." << endl;
        goto clientClose;
    }
 
    // Start query
    while(true)
    {
        if(courses.size() <= 1)
        {
            getline(cin, courseCode); // This getline is to clear the "\n" from last cin
        }
        courses.clear();
        allCourses = "";
        cout << "Please enter the course code to query: ";
        // cin >> courseCode;
        getline(cin, courseCode);
        // Check course code length to determine which mode to go
        if(courseCode.length() > 6)
        {
            // Need to get more than one courses' info
            istringstream inStream(courseCode);           
            while(inStream)
            {
                string temp;
                inStream >> temp;
                if(temp.length() >= 5)
                {
                    courses.push_back(temp);
                }
            }
            category = "all";
        }
        else
        {
            // Only one course needed
            courses.push_back(courseCode);
            cout << "Please enter the category (Credit / Professor / Days / CourseName): ";
            cin >> category;
        }
        
        // Check if more than 10 courses
        if(courses.size() >= 10)
        {
            cout << "ERROR: Cannot send query for more than 10 courses." << endl
                << "Please try again." << endl << endl;
            continue; 
        }

        for(int x = 0; x < courses.size(); x++)
        {
            // First send course code
            if(tcpClient->socSend(clientScoket, courses[x]) < 0)
            {
                cout << "Failed to send message from socket " << clientScoket << endl;
                exit(EXIT_FAILURE);
            }
            cout << username << " sent a request to the main server." << endl;
            // Then send request for info
            if(tcpClient->socSend(clientScoket, category) < 0)
            {
                cout << "Failed to send message from socket " << clientScoket << endl;
                exit(EXIT_FAILURE);
            }
            cout << username << " sent a request to the main server." << endl;
            // Receive query info from server
            if(tcpClient->socReceive(clientScoket, bufferTCP, BUFFER_SIZE) < 0)
            {
                cout << "Failed to receive message from socket " << clientScoket << endl;
                exit(EXIT_FAILURE);
            }
            string courseInfo = bufferTCP;
            cout << "The client received the response from the Main server using TCP over port " << portTCP << endl;
            if(courseInfo.compare("NULL") == 0)
            {
                // Course Info Not Found
                cout << "Didn’t find the course: " << courses[x] << endl;
            }
            else
            {
                if(courses.size() < 2)
                {
                    cout << "The " << category << " of " << courses[x] << " is " << courseInfo << "." << endl;
                }
                else
                {
                    allCourses += courses[x]+ ": " + courseInfo + "\n";
                }
            }
        }

        if(courses.size() >= 2)
        {
            cout << "CourseCode: Credits, Professor, Days, Course Name" << endl << allCourses << endl;
        }

        // Start next round of input
        cout << "\n\n" << "-----Start a new request-----" << endl;
    }

clientClose:
    delete [] bufferTCP;
    delete tcpClient;

    return 0;
}