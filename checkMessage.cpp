#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <ctime>
#include <chrono>

#include <iostream>
#include <sstream>
#include <iomanip>
#include "checkMessage.hpp"

checkMessage::checkMessage(){
    type = "";
    nodeNum = 0;
    confNodes = "";
}
void checkMessage::setType(string type1){
    type = type1;
}

void checkMessage::setNodeNum(int nodeNum1){
    nodeNum = nodeNum1;
}

void checkMessage::addNode(string newNode){
    confNodes += newNode;
}

string checkMessage::getType(){
    return type;
}

int checkMessage::getNodeNum(){
    return nodeNum;
}

string checkMessage::getConfNodes(){
    return confNodes;
}

string checkMessage::createMessage(string type1, int nodeNum1, string confNodes1){
    string message;
    message = "Type: " + type + "\n";
    message += "# of Nodes: "  + to_string(nodeNum1) + "\n";
    message += "Updated Nodes: " + confNodes1 + "\n";
    return message;
}


string checkMessage::typeFromMessage(string message){
    int i = 0;
    int j = 0;
    while (message.at(i) != *":"){ //moves through the string to get past "Type: "
        i++;
    } 

    i+=2; //skips the ":" char and space char 
    char array[50] = {'\0'}; // null terminated string to store the type name 
    while(message.at(i) != *"\n"){
        array[j] = message.at(i);
        j++;
        i++;
    }
    
    string output;
    output = array; //copies char arry to a string
    return output;
    
}

int checkMessage::nodeNumFromMessage(string message){
    int i = 0;
    int j = 0;
    char output;
    int out;
    while(message.at(i) != *"\n"){ //skips the first line
        i++;
    }
    i++;
    while (message.at(i) != *":"){ //moves through the string to get past "# of Nodes"
        i++;
    }
    i+=2;
    output = message.at(i);
    out = output - '0';
    return out;

}

string checkMessage::confNodesFromMessage(string message){
    int i = 0;
    int j = 0;
    string output;
    while(message.at(i) != *"\n"){ //skips the first line
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){ //skips the second line
        i++;
    }
    i++;
    while (message.at(i) != *":"){ //moves through the string to get past "Updated Nodes: "
        i++;
    }
    i+=2;
    char array[50] = {'\0'}; // null terminated string to store the type name 
    while(message.at(i) != *"\n"){
        array[j] = message.at(i);
        j++;
        i++;
    }
    
    output = array; //copies char arry to a string
    return output;
}

string checkMessage::createConfirm(){
    string out = "confirm\n";
}

string checkMessage::probeMessage(string type1, char curr){
    string out = "probe\n";
    out+= to_string(curr);
    return out;
}

int checkMessage::isThere(string nodes, char pres){
    int size = nodes.size();
	int out = -1;
	for(int i = 0; i < size; i++){
		if(nodes.at(i) == pres){
			out = 1;
		}
	}
	return out;
}

void checkMessage::dataMessagePrint(string input){
    int i = 0;

	cout << "Type: ";
	while(input.at(i) != *"\n"){
		cout << input;
		i++;
	}
	cout << "\n";

	cout << "Time: ";
	while(input.at(i) != *"\n"){
		cout << input;
		i++;
	}
	cout << "\n";

	cout << "Source: ";

	while(input.at(i) != *"\n"){
		cout << input;
		i++;
	}
	cout << "\n";
	
	cout << "Destination: ";

	while(input.at(i) != *"\n"){
		cout << input;
		i++;
	}
	cout << "\n";
}