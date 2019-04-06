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
#include "message.hpp"
#include "bellman.hpp"

#define DEFAULT_PORT "4000"
#define BUFFER_SIZE 1024
#define INFOAMT 14
#define NODEAMT 8

int
main(int argc, char *argv[])
{
	int router_fd, n_bytes, nodeCount = 0;
	struct sockaddr_storage their_addr;
	socklen_t their_addr_size;
	char ipstr[INET6_ADDRSTRLEN], buf[BUFFER_SIZE];
	std::string router_port = DEFAULT_PORT;
	timeval tv;
	bool firstNeighbour = true;
    int messageCount = 0;
    int notified = -1; //keeps track of whether thre user has been notified of convergence
    int adjNodes = 0;
    // Create empty vector for sockaddr structs
    std::vector<struct sockaddr *> other_routers;

    char *nodeName = argv[1];

    message DistanceVector;
    memset(&buf, 0, sizeof buf);
    n_bytes = strlen(buf);
    // Create empty graph for Bellman-Ford
    struct Graph* graph = bellmanSetup();



    //In the braces below is code that will setup a port with a known node name argument by reading the topology file,
    //it creates a table with Nodes and their ports, it will create a new node & assign it the inputted port and neighbours
    //(some functionality needs to be added to this to add Bellman Ford link costs)

        //nodeAndPort is a table with the node name and their port and their neighbours
        //<name, port, neighbour1 name, neighbour1 cost,..,neighbour6 name, neighbour6 cost>

        std::string nodeAndPort[NODEAMT][INFOAMT] =
                                        {{"A", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"B", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"C", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"D", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"E", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"F", "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"",  "", "", "", "", "", "", "", "", "", "", "", "", ""},
                                         {"",  "", "", "", "", "", "", "", "", "", "", "", "", ""}};
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
                //Sets up node neighbours and costs to them from file
                for(int checkNeighbours = 0 ; checkNeighbours < 6 ; checkNeighbours ++){
                    if(nodeAndPort[checkNeighbours][0][0] == topologyString[0] && nodeAndPort[checkNeighbours][0] == argv[1]){
                        for(int numNeigbours = 2 ; numNeigbours < 10 ; numNeigbours++){
                            if(nodeAndPort[checkNeighbours][numNeigbours] == "") {
                                if (firstNeighbour) {
                                    nodeAndPort[checkNeighbours][numNeigbours] = topologyString[2];
                                    firstNeighbour = false;
                                    break;
                                }
                                else{
                                    if(nodeAndPort[checkNeighbours][numNeigbours-1] == ""){
                                        nodeAndPort[checkNeighbours][numNeigbours] = topologyString[2];
                                        break;
                                    }
                                }
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
                
                switch (*nodeName) {
                    case 'A' :
                        router_port = nodeAndPort[0][1];
                        nodeCount = 0;
                        break;
                    case 'B' :
                        router_port = nodeAndPort[1][1];
                        nodeCount = 1;
                        break;
                    case 'C' :
                        router_port = nodeAndPort[2][1];
                        nodeCount = 2;
                        break;
                    case 'D' :
                        router_port = nodeAndPort[3][1];
                        nodeCount = 3;
                        break;
                    case 'E' :
                        router_port = nodeAndPort[4][1];
                        nodeCount = 4;
                        break;
                    case 'F' :
                        router_port = nodeAndPort[5][1];
                        nodeCount = 5;
                    default :
                        perror("Unknown node requested");
                        exit(1);
                }

                router_fd = setup_socket_udp(router_port.c_str());

                //Connects node to neighbours
                for(int addNeighbours = 0 ; addNeighbours < NODEAMT ; addNeighbours ++){
                    if(nodeName == nodeAndPort[addNeighbours][0]){
                        for(int neighbour = 2 ; neighbour < INFOAMT ; neighbour++){
                            if(nodeAndPort[addNeighbours][neighbour] != "" && neighbour % 2 == 0){
                                for(int neighbourPort = 0 ; neighbourPort < INFOAMT ; neighbourPort ++) {
                                    if(nodeAndPort[addNeighbours][neighbour] == nodeAndPort[neighbourPort][0]) {
                                        other_routers.push_back(get_address("localhost",
                                                nodeAndPort[neighbourPort][1].c_str())->ai_addr);
                                    }
                                }
                            }
                        }
                        //strcpy(buf,(DistanceVector.createControlHeader(nodeName, nodeAndPort[addNeighbours], INFOAMT)).c_str());
                        break;
                    }
                }
                break;
            }

            case 7 : {

                //New node inputted
                //Argument structure for new Node:
                //"<Name> <Port> <Neighbour 1 Name> <Neighbour 1 Cost> <Neighbour 2 Name> <Neighbour 2 Cost>"

                nodeCount = 6;

                for (int newAssign = 0 ; newAssign < 6 ; newAssign++){
                    nodeAndPort[6][newAssign] = argv[newAssign+1];
                }


                router_port = argv[2];
                router_fd = setup_socket_udp(router_port.c_str());

                for(int newNeighbour = 0 ; newNeighbour < NODEAMT ; newNeighbour ++){
                    if(nodeAndPort[newNeighbour][0] == nodeAndPort[6][2] || nodeAndPort[newNeighbour][0] == nodeAndPort[6][4]){
                        other_routers.push_back(get_address("localhost",
                                                            nodeAndPort[newNeighbour][1].c_str())->ai_addr);
                    }
                }
                //strcpy(buf,(DistanceVector.createControlHeader(argv[1], nodeAndPort[6], INFOAMT)).c_str());
                break;
            }
            default:
                fprintf(stderr, "usage: ./router [port]\n");
                exit(0);
        }


    

    // Wait for connections and deal with them
	while(1){
        if(messageCount < adjNodes*6 && !notified){
            cout << "Updating...\n";
        }
        else{
            cout << "All tables have been updated.\n";
        }
        
		if(!other_routers.empty()){
            
		    //Send cycle

		    //Send to all neighbours
            //Set up message in switch statement?..
            //Send to all as long as normal
            //Get data sending mode (Graham) - convergence

			// Create message and set byte count


            //Reset buffer
            memset(&buf, 0, sizeof buf);
            //Recheck buffer
            strcpy(buf,(DistanceVector.createControlHeader(argv[1], nodeAndPort[nodeCount], INFOAMT)).c_str());
            //sprintf(buf, "I am a router, located at port %s", argv[1]);
            n_bytes = strlen(buf);


			//printf("\nMessage [%dB]: %s\nRouter [%s] Sending to...\n\n", n_bytes, buf, argv[1]);
			std::cout << "Message Sent:\n" << buf;
			
			for(auto router : other_routers){
				
				// Print the other router's address and port details
				inet_ntop(router->sa_family, get_in_addr(router), ipstr, sizeof ipstr);
				//std::cout << ipstr << ":" << ntohs(get_in_port(router)) << std::endl;

				//Send message (from buf) to all in mailing list (Neighbours)
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
		
		//printf("\nRouter [%s]\nReceiving from...\n\n", argv[1]);

		do
		{
			// Break and go to send routine if false (timeout)	
			if(!wait_for_packet(router_fd, &tv)){ break; }

            // Populate graph with lowest link costs
            bellmanSetupFile(graph, nodeAndPort);


            // Reset buffer
            memset(&buf, 0, sizeof buf);
            n_bytes = 0;
            their_addr_size = sizeof their_addr;

            // Read in available data from buffer and do stuff
			n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
					   (struct sockaddr *)&their_addr, &their_addr_size);
			if(n_bytes == -1) { perror("recvfrom"); exit(0); }



			//Add parsing of data and Update 2D array
			//Pass to Bellman Ford algorithm
			//For testing can add if(oldCost > newCost) = change 2D array

            //Remember file I/O



			// Print the other router's address and port details
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);

			//std::cout << ipstr << ":" << ntohs(get_in_port((struct sockaddr *)&their_addr)) <<
			//" [" << n_bytes << "B] " << buf << std::endl;

		    std::cout << "Message Received:\n"<< buf;
			
           	if(DistanceVector.parseType(buf) == "Control") {
                // Number of DV lines
                int num_DVs = 0;
                //The update DVs for Bellman Ford are stored in DV
                std::string **DV = DistanceVector.parseDV(buf, &num_DVs);

                bellmanUpdateFile(graph, DV, num_DVs);
                adjNodes = num_DVs;
                if(!bellmanUpdateArray(nodeAndPort)){
                    messageCount++;
                    
                }
                else{
                    cout << "Update detected!\n";
                    messageCount = 0;
                    notified = -1;
                }
            }
		    else{ //received DataMessage
		        char source = DistanceVector.parseSource(buf);
                if(source == *nodeName){
                    DistanceVector.printHeaderMessage(buf);
                    cout << "\n";
                    DistanceVector.printData(buf);
                    cout << "\n";
                }
                else{
                    //have to use Bellman Ford to send Data to next closest node
                }
                
		    }

            // Check if new router, and add to list
			add_new_addr((struct sockaddr *)&their_addr, other_routers);
		}
		while((tv.tv_sec > 0) && (tv.tv_usec > 0));


	
	return 0;
    }
}