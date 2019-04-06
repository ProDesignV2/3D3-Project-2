#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fstream>

#include "message.hpp"
#include "bellman.hpp"

#define BUFFER_SIZE 1024
#define INFOAMT 14
#define NODEAMT 8

int
main(int argc, char *argv[])
{
	int n_bytes, nodeCount = 0;
	char buf[BUFFER_SIZE];

    message DistanceVector;
    memset(&buf, 0, sizeof buf);
    n_bytes = strlen(buf);

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
                if(nodeAndPort[checkNeighbours][0][0] == topologyString[0]){
                    for(int numNeigbours = 2 ; numNeigbours < 10 ; numNeigbours++){
                        if(nodeAndPort[checkNeighbours][numNeigbours] == ""){
                            nodeAndPort[checkNeighbours][numNeigbours] = topologyString[2];
                            numNeigbours++;
                            nodeAndPort[checkNeighbours][numNeigbours] = topologyString[10];
                            break;
                        }
                    }
                }
            }
        }
    }

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
                    nodeCount = 0;
                    break;
                case 'B' :
                    nodeCount = 1;
                    break;
                case 'C' :
                    nodeCount = 2;
                    break;
                case 'D' :
                    nodeCount = 3;
                    break;
                case 'E' :
                    nodeCount = 4;
                    break;
                case 'F' :
                    nodeCount = 5;
                    break;
                default :
                    perror("Unknown node requested");
                    exit(1);
            }

            break;
        }

        default:
            fprintf(stderr, "usage: ./router [port]\n");
            exit(0);
    }

    for(int i = 0; i < NODEAMT; i++){
        for(int j = 0; j < INFOAMT; j++){
            std::cout << nodeAndPort[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // //Reset buffer
    // memset(&buf, 0, sizeof buf);
    // //Recheck buffer
    // strcpy(buf,(DistanceVector.createControlHeader(argv[1], nodeAndPort[nodeCount], INFOAMT)).c_str());
    // //sprintf(buf, "I am a router, located at port %s", argv[1]);
    // n_bytes = strlen(buf);

    // std::cout << "Message Sent:\n" << buf;
    
    // // Sending to router
    // // Receiving at router

    // std::cout << "Message Received:\n"<< buf;
    
    // if(DistanceVector.parseType(buf) == "Control") {

    //     // Counter for DV lines
    //     int num_DVs = 0;

    //     //The update DVs for Bellman Ford are stored in DV
    //     std::string **DV = DistanceVector.parseDV(buf, &num_DVs);

    //     std::cout << "START " << num_DVs << std::endl;
    //     for(int i = 0; i < num_DVs; i++){
    //         std::cout << DV[i][0] << DV[i][1] << DV[i][2] << std::endl;
    //     }
    //     std::cout << "END" << std::endl;

    //     // Create graph - this should be a global variable
    //     struct Graph* graph = bellmanSetup();
    //     // Update graph file
    //     num_DVs = 0;
    //     bellmanUpdateFile(graph, DV, num_DVs);
    //     // Update graph array with any lower costs
    //     bellmanUpdateArray(nodeAndPort);
    // }

    struct Graph* graph = bellmanSetup();
    bellmanSetupFile(graph, nodeAndPort);
    bellmanUpdateArray(nodeAndPort);

    for(int i = 0; i < NODEAMT; i++){
        for(int j = 0; j < INFOAMT; j++){
            std::cout << nodeAndPort[i][j] << " ";
        }
        std::cout << std::endl;
    }

	return 0;
}
