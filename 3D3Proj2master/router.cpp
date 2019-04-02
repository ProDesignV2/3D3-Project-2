#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fstream>

#include "helper.h"

#define DEFAULT_PORT "4000"
#define BUFFER_SIZE 1024

int
main(int argc, char *argv[])
{
	int router_fd, n_bytes;
	struct sockaddr_storage their_addr;
	socklen_t their_addr_size;
	char ipstr[INET6_ADDRSTRLEN], buf[BUFFER_SIZE];
	std::string router_port = DEFAULT_PORT;
	timeval tv;

    // Create empty vector for sockaddr structs
    std::vector<struct sockaddr *> other_routers;

	


    //In the braces below is code that will setup a port with a known node name argument by reading the topology file,
    //it creates a table with Nodes and their ports, it will also take in a new node and assign it the inputted port
    //(some functionality needs to be added to this to deal with Bellman ford)
    {
        //nodeAndPort is a table with the node name and their port and their neighbours
        std::string nodeAndPort[8][8] = {{"A", "", "", "", "", "", "", ""},
                                         {"B", "", "", "", "", "", "", ""},
                                         {"C", "", "", "", "", "", "", ""},
                                         {"D", "", "", "", "", "", "", ""},
                                         {"E", "", "", "", "", "", "", ""},
                                         {"F", "", "", "", "", "", "", ""},
                                         {"",  "", "", "", "", "", "", ""},
                                         {"",  "", "", "", "", "", "", ""}};
        //fileName is the name of the topology file and topFile is the accessible instantiation of the file
        std::string fileName = "FileTopology";
        std::fstream topFile;



        //Check if topology file is present and open it
        topFile.open(fileName, std::ios_base::in);
        if (!topFile.is_open()) {
            perror("fstream : file won't open");
            exit(1);
        }


        //Convert the topology file into a  table  with name, port and neighbours (A 10000, B E...)
        std::string topologyString = "";
        while (topFile.good()) {
            getline(topFile, topologyString);
            if (topologyString.size() > 0) {
                switch (topologyString[2]) {
                    case 'A' :
                        nodeAndPort[0][1] = topologyString.substr(4, 5);
                        break;
                    case 'B' :
                        nodeAndPort[1][1] = topologyString.substr(4, 5);
                        break;
                    case 'C' :
                        nodeAndPort[2][1] = topologyString.substr(4, 5);
                        break;
                    case 'D' :
                        nodeAndPort[3][1] = topologyString.substr(4, 5);
                        break;
                    case 'E' :
                        nodeAndPort[4][1] = topologyString.substr(4, 5);
                        break;
                    case 'F' :
                        nodeAndPort[5][1] = topologyString.substr(4, 5);
                        break;
                    default :
                        perror("Non standard file input");
                        exit(2);
                }
                //Sets up node neighbours
                for(int checkNeighbours = 0 ; checkNeighbours < 6 ; checkNeighbours ++){
                    if(nodeAndPort[checkNeighbours][0][0] == topologyString[0]){
                        for(int numNeigbours = 2 ; numNeigbours < 6 ; numNeigbours++){
                            if(nodeAndPort[checkNeighbours][numNeigbours] == ""){
                                nodeAndPort[checkNeighbours][numNeigbours] = topologyString[2];
                                break;
                            }
                        }
                    }
                }
            }
        }



        //Check if arg[1] == A/B/.../F if so
        //Set command argument as router port if entered

        switch (argc) {

            //Need to implement randomising a known port and checking if that port is free
            case 1: {
                fprintf(stderr, "usage: ./router [port]\n");
                exit(0);

            }

            //Checks node name argument and sets the port accordingly
            case 2: {
                char *nodeName = argv[1];
                switch (*nodeName) {
                    case 'A' :
                        router_port = nodeAndPort[0][1];
                        break;
                    case 'B' :
                        router_port = nodeAndPort[1][1];
                        break;
                    case 'C' :
                        router_port = nodeAndPort[2][1];
                        break;
                    case 'D' :
                        router_port = nodeAndPort[3][1];
                        break;
                    case 'E' :
                        router_port = nodeAndPort[4][1];
                        break;
                    case 'F' :
                        router_port = nodeAndPort[5][1];
                        break;
                    default :
                        perror("Unknown node requested");
                        exit(1);
                }
                router_fd = setup_socket_udp(router_port.c_str());

                //Connects node to neighbours
                for(int addNeighbours = 0 ; addNeighbours < 6 ; addNeighbours ++){
                    if(nodeName == nodeAndPort[addNeighbours][0]){
                        for(int neighbour = 2 ; neighbour < 6 ; neighbour ++){
                            if(nodeAndPort[addNeighbours][neighbour] != ""){
                                for(int neighbourPort = 0 ; neighbourPort < 6 ; neighbourPort ++) {
                                    if(nodeAndPort[addNeighbours][neighbour] == nodeAndPort[neighbourPort][0]) {
                                        other_routers.push_back(get_address("localhost",
                                                                            nodeAndPort[neighbourPort][1].c_str())->ai_addr);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            case 7 : {

                //New node inputted
                //Argument structure for new Node:
                //"<Name> <Port> <Neighbour 1 Name> <Neighbour 1 Cost> <Neighbour 2 Name> <Neighbour 2 Cost>"


                nodeAndPort[6][0] = argv[1];    //Node Name
                nodeAndPort[6][1] = argv[2];    //Port
                nodeAndPort[6][2] = argv[3];    //Neighbour 1
                nodeAndPort[6][3] = argv[5];    //Neighbour 2

                router_port = argv[2];
                router_fd = setup_socket_udp(router_port.c_str());

                for(int newNeighbour = 0 ; newNeighbour < 8 ; newNeighbour ++){
                    if(nodeAndPort[newNeighbour][0] == nodeAndPort[6][2] || nodeAndPort[newNeighbour][0] == nodeAndPort[6][3]){
                        other_routers.push_back(get_address("localhost",
                                                            nodeAndPort[newNeighbour][1].c_str())->ai_addr);
                    }
                }
                break;
            }
            default:
                fprintf(stderr, "usage: ./router [port]\n");
                exit(0);
        }
    }





    // Wait for connections and deal with them
	while(1){
	
		if(!other_routers.empty()){

			// Create message and set byte count
			memset(&buf, 0, sizeof buf);
			sprintf(buf, "I am a router, located at port %s", argv[1]);
			n_bytes = strlen(buf);

			printf("\nMessage [%dB]: %s\nRouter [%s] Sending to...\n\n", n_bytes, buf, argv[1]);
			
			for(auto router : other_routers){
				
				// Print the other router's address and port details
				inet_ntop(router->sa_family, get_in_addr(router), ipstr, sizeof ipstr);
				std::cout << ipstr << ":" << ntohs(get_in_port(router)) << std::endl;

				their_addr_size = sizeof their_addr;
				if (send_all(router_fd, buf, &n_bytes, router, their_addr_size) == -1) {
					perror("send_all");
					continue;
				}
			}
		}

		// Reset timeout for receiving data
		tv.tv_sec = TIMEOUT_SEC;
		tv.tv_usec = TIMEOUT_USEC;
		
		printf("\nRouter [%s]\nReceiving from...\n\n", argv[1]);

		do
		{
			// Break and go to send routine if false (timeout)	
			if(!wait_for_packet(router_fd, &tv)){ break; }


			// Read in available data from buffer and do stuff

			// Reset buffer
			memset(&buf, 0, sizeof buf);
			n_bytes = 0;	
			their_addr_size = sizeof their_addr;

			n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
					   (struct sockaddr *)&their_addr, &their_addr_size);
			if(n_bytes == -1) { perror("recvfrom"); exit(0); }

			// Print the other router's address and port details
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);
			std::cout << ipstr << ":" << ntohs(get_in_port((struct sockaddr *)&their_addr)) <<
			" [" << n_bytes << "B] " << buf << std::endl; 
		
			// Check if new router, and add to list	
			add_new_addr((struct sockaddr *)&their_addr, other_routers);
		}
		while((tv.tv_sec > 0) && (tv.tv_usec > 0));

	}
	
	return 0;
}
