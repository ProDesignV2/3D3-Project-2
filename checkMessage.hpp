#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <string>

using namespace std;

//purpose of this class is to create a message that is sent after a period of time
//by the last created node, to test that all nodes have the updated file.
//the message contained the type of message it is, i.e. a check message in
//this case, the number of nodes that should be in the network and
// a list of updated nodes.
//The first node will send it to each of it's neighbours, and each 
//node that receives it will add itself to the list of nodes if it is
//not there or ignore it if it is. When a node adds itself to the list of
//nodes, it also checks the length of the list of list of nodes and 
//compares it to the number of nodes specified in the message.
//if they are the same, then the node knows that all the nodes have been 
//added to the list. It then sends a confirmation message to all it's nodes, 
//and each node sends it to all their neighbours. Each node is instructed to send 
//this confirmation message once. 
class checkMessage{
    public:
        
        checkMessage();
        //these functions used to set values in the message
        void setType(string type1);
        void setNodeNum(int nodeNum1);
        void addNode(string newNode);
        //these functions used to get values from the message
        string getType();
        int getNodeNum();
        string getConfNodes();
        //creates the message as a string, in order to be sent from socket
        string probeMessage(string type1, char curr);
        string createMessage(string type1, int nodeNum1, string confNodes1);
        //parses the string received into the values needed.
        string typeFromMessage(string message); //takes the string input and returns the type.
        int nodeNumFromMessage(string message); //takes the string input and returns the # of nodes.
        string confNodesFromMessage(string message); //takes the string input and returns the # of conf nodes.
        //function below will create a confirmation message that is sent to all nodes
        string createConfirm(); //message sent when all nodes are updated;
        int isThere(string nodes, char pres);
        void dataMessagePrint(string input);
    private:
         string type; //this used to  indicate it is a check message
        int nodeNum; //number of nodes
        string confNodes;
        //String of all nodes that have received this message and
        //checked if they are up to date.
};
