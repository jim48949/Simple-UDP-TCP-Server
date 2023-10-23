
all : client.o clientTCP.o clientUDP.o serverTCP.o serverUDP.o serverM.o serverC.o serverCS.o serverEE.o
	sudo g++ -std=c++11 client.o clientTCP.o -o client
	sudo g++ -std=c++11 serverM.o serverUDP.o clientUDP.o serverTCP.o -o serverM
	sudo g++ -std=c++11 serverC.o serverUDP.o clientUDP.o -o serverC
	sudo g++ -std=c++11 serverCS.o serverUDP.o clientUDP.o -o serverCS
	sudo g++ -std=c++11 serverEE.o serverUDP.o clientUDP.o -o serverEE
	sudo chmod -R 777 *

clean: 
	sudo rm -rf *.o *.out client clientTCP clientUDP serverTCP serverUDP serverM serverC serverCS serverEE

client.o : client.cpp
	sudo g++ -std=c++11 -c client.cpp

serverM.o : serverM.cpp
	sudo g++ -std=c++11 -c serverM.cpp

serverC.o : serverC.cpp
	sudo g++ -std=c++11 -c serverC.cpp

serverCS.o : serverCS.cpp
	sudo g++ -std=c++11 -c serverCS.cpp

serverEE.o : serverEE.cpp
	sudo g++ -std=c++11 -c serverEE.cpp

clientTCP.o : clientTCP.cpp
	sudo g++ -std=c++11 -c clientTCP.cpp

clientUDP.o : clientTCP.cpp
	sudo g++ -std=c++11 -c clientUDP.cpp

serverTCP.o : serverTCP.cpp
	sudo g++ -std=c++11 -c serverTCP.cpp

serverUDP.o : serverUDP.cpp
	sudo g++ -std=c++11 -c serverUDP.cpp