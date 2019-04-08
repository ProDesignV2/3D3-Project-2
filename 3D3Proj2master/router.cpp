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
#include <ctime>

#include "helper.h"
#include "message.hpp"
#include "bellman.hpp"

#define DEFAULT_PORT "4000"
#define BUFFER_SIZE 1024
#define INFOAMT 14
#define NODEAMT 8
#define DISTIMEOUT 5

int
main(int argc, char *argv[])
{
	int router_fd, n_bytes, nodeCount = 0;
	struct sockaddr_storage their_addr;
	socklen_t their_addr_size;
	char ipstr[INET6_ADDRSTRLEN], buf[BUFFER_SIZE], nodesPresent[NODEAMT], checkNode[NODEAMT], neighbours[NODEAMT], disflag, clonePresent[NODEAMT];
	std::string router_port = DEFAULT_PORT, cloneDV[NODEAMT][3];
	timeval tv;
	bool convFlag= true, disapflag = false;
	int disappearance, disCount= 0;
	char disappearingFlag = '0', disTimeOfDeath = '0', disClone = '0';
	memset(nodesPresent, 0 , sizeof nodesPresent);
    memset(checkNode, 0 , sizeof checkNode);
    memset(neighbours, 0, sizeof neighbours);
    char *nodeName;

    int messageCount = 0;
    int notified = -1; //keeps track of whether thre user has been notified of convergence


    std::string filename = "routing-output";
    filename += argv[1];
    filename.append(".txt");
    std::fstream routerOutput;
    routerOutput.open(filename, std::ios_base::trunc | std::ios_base::out);
    if(!routerOutput.is_open()) {
        perror("Won't open output file");
    }
    routerOutput.close();
    routerOutput.open(filename, std::ios_base::app);
    if(!routerOutput.is_open()){
        perror("Won't open output file");
    }


    // Create empty vector for sockaddr structs
    std::vector<struct sockaddr *> other_routers;


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

        std::string clonetable[NODEAMT][INFOAMT], nodeAndPort[NODEAMT][INFOAMT] =
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

        int index = 0;
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
                                nodeAndPort[checkNeighbours][numNeigbours] = topologyString[2];
                                neighbours[index] = topologyString[2];
                                index++;
                                numNeigbours++;
                                nodeAndPort[checkNeighbours][numNeigbours] = topologyString[10];
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
                nodeName = argv[1];
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
                        break;
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
                                for(int neighbourPort = 0 ; neighbourPort < NODEAMT ; neighbourPort ++) {
                                    if(nodeAndPort[addNeighbours][neighbour] == nodeAndPort[neighbourPort][0]) {
                                        other_routers.push_back(get_address("localhost",
                                                nodeAndPort[neighbourPort][1].c_str())->ai_addr);
                                    }
                                }
                            }
                        }
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
                neighbours[0] = argv[2][0];
                neighbours[1] = argv[4][0];


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


    disappearance = 0;

    // Wait for connections and deal with them
	while(1){
	
		if(!other_routers.empty()){

		    //Send cycle

		    //Send to all neighbours
            //Set up message in switch statement?..
            //Send to all as long as normal
            //Get data sending mode (Graham) - convergence


            //Reset buffer
            memset(&buf, 0, sizeof buf);
            //Recheck buffer
            strcpy(buf,(DistanceVector.createControlHeader(argv[1], nodeAndPort[nodeCount], INFOAMT, nodesPresent, disappearingFlag, disTimeOfDeath)).c_str());
            n_bytes = strlen(buf);



			std::cout << "Message Sent:\n" << buf;
			
			for(auto router : other_routers){
				
				// Print the other router's address and port details
				inet_ntop(router->sa_family, get_in_addr(router), ipstr, sizeof ipstr);

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




		if(disTimeOfDeath == '0') {
            disappearingFlag = '0';
        }
		do
		{
			// Break and go to send routine if false (timeout)	
			if(!wait_for_packet(router_fd, &tv)){ break; }




            // Reset buffer
            memset(&buf, 0, sizeof buf);
            n_bytes = 0;
            their_addr_size = sizeof their_addr;

            // Read in available data from buffer and do stuff
			n_bytes = recvfrom(router_fd, buf, sizeof buf, 0,
					   (struct sockaddr *)&their_addr, &their_addr_size);
			if(n_bytes == -1) { perror("recvfrom"); exit(0); }

			int countPresence = 0;
			char checkPresence = DistanceVector.parseDataSource(buf);
			for(int i = 0 ; i < NODEAMT ; i++) {
                if (neighbours[i] != checkPresence){
                    countPresence++;
                }
			}
			if(countPresence == 8){
			    for(int j = 0 ; j < NODEAMT ; j++){
			        if(neighbours[j] == 0){
			            neighbours[j] = checkPresence;
                        nodeAndPort[6][0][0] = checkPresence;
			            break;
			        }
			    }
			}




			//Add parsing of data and Update 2D array
			//Pass to Bellman Ford algorithm
			//For testing can add if(oldCost > newCost) = change 2D array

            //Remember file I/O



			// Print the other router's address and port details
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), ipstr, sizeof ipstr);

//			for(int i =0 ; i < NODEAMT ; i++){
//			    clonePresent[i] = nodesPresent[i];
//			}


			//Allows neighbours to be added
			int count = 0;
            for(int i = 0 ; i < NODEAMT ; i ++){
                if(nodesPresent[i] != DistanceVector.parseDataSource(buf)){
                    count ++;
                }
            }
            //Adds message source to present node table
            if(count == 8){
                for(int j = 0 ; j < NODEAMT ; j++){
                    if(nodesPresent[j] == 0){
                        nodesPresent[j] = DistanceVector.parseDataSource(buf);
                        break;
                    }
                }
            }


            // Number of DV lines
            int num_DVs = 0, counter=0;
            std::string **DV;
            char flag;
            DV = DistanceVector.parseDV(buf, &num_DVs);
            for (int k = 1 ; k <= num_DVs ; k++ ){
                flag = DistanceVector.parseOtherNodes(DV, k);
                for(int i = 0 ; i < NODEAMT ; i++) {
                    if(flag != nodesPresent[i]){
                        counter ++;
                    }
                }
                if(counter == 8){
                    for(int j = 0 ; j < NODEAMT ; j++){
                        if(nodesPresent[j] == 0){
                            if(disCount == 0) {
                                nodesPresent[j] = flag;
                            }
                            break;
                        }
                    }
                }
                counter = 0;
            }




            disflag = DistanceVector.passSource(buf);
            if (disTimeOfDeath == '1') {
                disTimeOfDeath = '0';
            }

            if (disflag == 'A' || disflag == 'B' || disflag == 'C' || disflag == 'D' || disflag == 'E' ||
                disflag == 'F' || disflag == 'G') {
                    disTimeOfDeath = DistanceVector.passLength(buf) - 1;
                for (int i = 0; i < NODEAMT; i++) {
                    if (nodesPresent[i] == disflag) {
                        disapflag = true;
                        disappearingFlag = disflag;
                        nodesPresent[i] = 0;
                        disCount = 2;
                    }
                }
            }




            // Populate graph with lowest link costs
            bellmanSetupFile(graph, nodeAndPort, nodesPresent);


		    std::cout << "Message Received:\n"<< buf;
            if(messageCount > 10){
                cout << "All tables have been updated.\n";
                notified = 1;
            }
            else{
                cout << "Updating...\n";
            }


            if (disappearingFlag == 'A' || disappearingFlag == 'B' || disappearingFlag == 'C' || disappearingFlag == 'D'
            || disappearingFlag == 'E' ||disappearingFlag == 'F' || disappearingFlag == 'G'){
                if(disCount == 2) {
                    routerOutput << "Node " << disappearingFlag
                                 << " has been removed from the network.\nIts values will be kept in the table.\n";
                    routerOutput << "\n";
                    routerOutput.flush();
                }
            }

			
           	if(DistanceVector.parseType(buf) == "Control") {


                for(int i = 0 ; i < NODEAMT ; i++){
                    for(int j = 0 ; j < 3 ; j++){
                        cloneDV[i][j] = DV[i][j];
                    }
                }

                //The update DVs for Bellman Ford are stored in DV
                //DV = DistanceVector.parseDV(buf, &num_DVs);
                for(int i = 0 ; i < INFOAMT ; i ++){
                    for(int j = 0 ; j < NODEAMT ; j ++){
                        clonetable[j][i] = nodeAndPort[j][i];
                    }
                }




                bellmanUpdateFile(graph, DV, num_DVs);
                if(bellmanUpdateArray(nodeAndPort)){
                    routerOutput << DistanceVector.parseTime(buf);
                    routerOutput << "\n";
                    routerOutput << "\n";
                    for(int i = 0 ; i < NODEAMT ; i++){
                        for(int j = 0 ; j < INFOAMT ; j++){
                            if(clonetable[i][j] == ""){
                                break;
                            }
                            routerOutput << clonetable[i][j];
                            routerOutput << ",";
                        }
                        routerOutput << "\n";
                    }

                    for(int i = 0 ; i < NODEAMT ; i++){
                        for(int j = 0 ; j < 3 ; j++){
                            if(cloneDV[i][j] == "" && i == 0 && j == 0){
                                routerOutput << "Update caused by addition of neighbour node (costs read from topology file).";
                                break;
                            }
                            if(cloneDV[i][j] == ""){
                                break;
                            }
                            routerOutput << cloneDV[i][j];
                            routerOutput << ",";
                        }
                    }
                    routerOutput << "\n";
                    routerOutput << "\n";
                    for(int i = 0 ; i < NODEAMT ; i++){
                        for(int j = 0 ; j < INFOAMT ; j++){
                            if(nodeAndPort[i][j] == ""){
                                break;
                            }
                            routerOutput << nodeAndPort[i][j];
                            routerOutput << ",";
                        }
                        routerOutput << "\n";
                    }
                    routerOutput << "\n";
                    routerOutput.flush();
                }
                else{
                    bellmanUpdateArray(nodeAndPort);
                    messageCount++;
                    convFlag = false;
                }
                int flag = NODEAMT;
                for(int i = 0 ; i < NODEAMT ; i ++) {
                    if(clonePresent[i] != nodesPresent[i]){
                        flag--;
                    }
                    if (convFlag && flag != NODEAMT || disapflag) {
                        cout << "Update detected!\n";
                        bellmanUpdateArray(nodeAndPort);
                        messageCount = 0;
                        disapflag = false;
                        break;
                    }
                }
                for(int i =0 ; i < NODEAMT ; i++){
                    clonePresent[i] = nodesPresent[i];
                }
                convFlag = true;
            }
            else { //data message
                char dest = DistanceVector.parseDataDest(buf); //parses destination node
                if (dest == *nodeName) {
                    DistanceVector.printHeaderMessage(buf);
                    cout << "\n";
                    DistanceVector.printData(buf);
                    cout << "\n";
                } else {
                    int xpos = *nodeName - 65;
                    int ypos = dest - 65;
                    DistanceVector.printHeaderMessage(buf); //prints header message each time a node recieves data.
                    string nextNode = nodeAndPort[xpos][ypos];
                    char portnum[10] = {'\0'};
                    //need to run BF again to determine next node.
                    //then send buf to next node on path
                }
            }


            switch(DistanceVector.parseDataSource(buf)) {
                case 'A':
                    checkNode[0] = 1;
                    break;
                case 'B':
                    checkNode[1] = 1;
                    break;
                case 'C':
                    checkNode[2] = 1;
                    break;
                case 'D':
                    checkNode[3] = 1;
                    break;
                case 'E':
                    checkNode[4] = 1;
                    break;
                case 'F':
                    checkNode[5] = 1;
                    break;
                case 'G':
                    checkNode[6] = 1;
                    break;
		        default :
                    checkNode[7] = 1;
                    //std::cout << "default on disappearance\n";
                    break;
            }




            if( disappearance > DISTIMEOUT){


		        //Check if any neighbours == 0 and remove if so
		        for(int i = 0 ; i < NODEAMT ; i++){
		            for(int j = 0 ; j < NODEAMT ; j++) {
                        if (checkNode[i] == 0 && nodeAndPort[i][0][0] == neighbours[j]){
                            for(int k  = 0 ; k < NODEAMT ; k++){
                                if(nodesPresent[k] == neighbours[j] && nodesPresent[k] != 0){
                                    disapflag = true;
                                    nodesPresent[k] = 0;
                                    disappearingFlag = neighbours[j];
                                    disCount = 3;
                                    disTimeOfDeath = '4';
                                }
                            }
                        }
                    }
		        }
                memset(checkNode, 0 , sizeof checkNode);
                disappearance = 0;
		    }

            // Check if new router, and add to list
			add_new_addr((struct sockaddr *)&their_addr, other_routers);



		}
		while((tv.tv_sec > 0) && (tv.tv_usec > 0));
        disappearance++;
        if(disCount > 0){
            disCount--;
        }

	}
	routerOutput.close();
	return 0;
}
