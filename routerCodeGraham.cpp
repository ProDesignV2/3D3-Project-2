#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "checkMessage.hpp"
#include "helper.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>


int main(){
    //a router is added.
    //following code runs on setup of router.
    string probe = "probe";
    string upd = "update";
    string confirm = "confirm";

    checkMessage mess;

    char myNode = *"A"; //this will be read in from command line along with connected nodes, ports and weights
    string nodes;
    nodes += to_string(myNode); //applies char to string 
    string message = mess.probeMessage(probe, myNode); //this message sent to all listed nodes in input

    int numNodes = 0;

    int updating = -1;
    
    while(1){
        //receives message
        while(updating){
            updating = 1;
            string input; //this would be replaced with message received on socket
            
            //parse string input 
            string type = mess.typeFromMessage(input);

            if(type.compare(probe)){
                numNodes++;
                string response = mess.createMessage(upd, numNodes, nodes);
                //send to connected nodes
            }

            if(type.compare(upd)){
                int num = mess.nodeNumFromMessage(input);
                numNodes = num; // sets number of nodes in network == number of nodes in updated 
                string confNodes = mess.confNodesFromMessage(input);
                if(!mess.isThere(confNodes, myNode)){
                    confNodes += nodes; //adds current node to list of confirmed nodes
                }
                
                if(num == numNodes){ //checks to see if num of confirmed nodes == total nodes (i.e, all updated)
                     string reponse = mess.createConfirm();
                }
            }

            if(type.compare(confirm)){
                updating = -1;
            }
        
        return 0;
        }
    }
}