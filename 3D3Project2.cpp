#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "checkMessage.hpp"

#include <iostream>
#include <sstream>

//each header file must contain:


int main(){
    //mess created below is meant to replicate a message from another node
    checkMessage mess;

    string upd = "update";

    string curr = "E";

    mess.setType("update");
    mess.setNodeNum(6);
    mess.addNode("A");

    string message;

    mess.addNode("B");
    

    mess.addNode("C");
    

    mess.addNode("D");
    //this message if for a netowrk with 6 nodes that has been confirmed by ABCD
    ///////////////////////////////////////////////////////////
    checkMessage newmess;

    string list = newmess.typeFromMessage(message);
    int num = newmess.nodeNumFromMessage(message);
    string nodes = newmess.confNodesFromMessage(message);
    
    if(list.compare(upd) == 0){
        

        newmess.addNode(nodes);
        
        newmess.setType(list);
        newmess.setNodeNum(num);
        newmess.addNode(curr);
         
    }


    //upon receipt of message, the node aprses it and makes a new instance of the checkMessage class
    //and adds the info to it

    //this allows the  

    //this is what the rough code for a node receiving this message should look like
    if(!isPresent(nodes, curr)){
        mess.addNode(curr);
    }
    if(allConfirmed(list, mess.getNodeNum())){
        //mess.createConfirmMessage(list); //need to write this function for cconfirmation message
    }
    
    
}
 //use this function to check if the length of the string is the same as the number of nodes in the network
 //when this happens, it means that all nodes have added themselves to the list of confirmed nodes, i.e. they have all
 //received the update.
int allConfirmed(string list, int num){
    if(num == list.length()){
        return 1;
    }
    else{
        return -1;
    }
    
}

int isPresent(string list, char curr){
    int size = list.length();
    int out = - 1;
    
    char arr[size+1];
    strcpy(arr, list.c_str());
    for(int i = 0; i < size; i++){
        if(arr[i] == curr){
            out = 1;
        }
    }
}