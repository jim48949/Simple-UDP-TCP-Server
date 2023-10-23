What this project does:
Implemented a simple web registration system for EE/CS departments, using both UDP and TCP and created a credential server to verify user's logging informations.
It is also allows user to enter more than one course as query.

Code files and their functionalities:
client.cpp: Client's interface that is used for login and get course's information.
clientTCP.cpp / clientTCP.h: TCP class on client's side for creating and using client's TCP sockcts.
clientUDP.cpp / clientUDP.h: UDP class on client's side for creating and using client's UDP sockcts.
serverTCP.cpp / serverTCP.h: TCP class on server's side for creating and using server's TCP sockcts.
serverUDP.cpp / serverUDP.h: TCP class on server's side for creating and using server's UDP sockcts.
serverC.cpp: Credentical server that is used for checking login credentials.
serverCS.cpp: CS department's server that is used for retriving CS courses' informations.
serverEE.cpp: EE department's server that is used for retriving EE courses' informations.
serverM.cpp: Main server that serves the functions of forwarding / receving TCP / UDP messages between the client and backend credential or department servers.
	     It also encrypts the login infos and parse the course queries from clients.

Code References: 
SOCKETS-SERVER & CLIENT-2020 C++ (https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
LOGIC https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf
