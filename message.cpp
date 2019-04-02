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
#include "message.hpp"

message::message(){
    
}

string message::createDataHeader(char *source, char *dest, int rport, int sport, string data){
    string out = "Data\n";
    out.append(source); 
    out += ",";
    out.append(dest);
    out += to_string(rport) + ", " + to_string(sport);
    out += data;

    return out;
}

string message::createControlHeader(int type){
    string out = "Control\n";
    out += to_string(type) + "\n";
}

string message::appendControl(string input, char *source, char *dest, int port, int weight){
    input.append(source);
    input += ",";
    input.append(dest);
    input += ",";
    input += to_string(port) + "," + to_string(weight) + "\n";
}

string message::parseType(string message){
    int i = 0;
    char array[15] = {'\0'};
    while(message.at(i) != *"\n"){
        array[i] = message.at(i);
        i++;
    }

    string output = array;
    return output;

}

int message::parseControlType(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    int j = 0;
    out = message.at(i);
    out = out - '0';
    return out;
}

char message::parseSource(string message, int num){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    for(int i = 1; i < num; i++){
       while(message.at(i) != *"\n"){
        i++;
        } //this skips down through the lines of connection info
    }
    i++;
    out = message.at(i);
    
    return out;
}


char message::parseDest(string message, int num){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    for(int i = 1; i < num; i++){
       while(message.at(i) != *"\n"){
        i++;
        }
    }
    i+3;
    out = message.at(i);
    return out;
}

int message::parsePort(string message, int num){
    int i = 0;
    char array[15] = {'\0'};
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    for(int i = 1; i < num; i++){
       while(message.at(i) != *"\n"){
        i++;
        }
    }
    i+5;
    int j = 0;
    while(message.at(i) != *","){
        array[j] = message.at(i);
        i++;
        j++;
    }
    
    int out = atoi(array);

    return out;
}

int message::parseWeight(string message, int num){
    int i = 0;
    char array[15] = {'\0'};
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    for(int i = 1; i < num; i++){
       while(message.at(i) != *"\n"){
        i++;
        }
    }
    i+5;
    while(message.at(i) != *","){
        i++;
    }
    int j = 0;
    while(message.at(i) != *"\n"){
        array[j] = message.at(i);
        i++;
        j++;
    }
    
    int out = atoi(array);

    return out;
    
}

char message::parseDataSource(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    out = message.at(i);

    return out;
}

char message::parseDataDest(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i+3;
    out = message.at(i);

    return out;
}

int message::parseInPort(string message){
    int i = 0;
    char out[15] = {'\0'};
    
    while(message.at(i) != *"\n"){
        i++;
    }
    i+5;
    int j = 0;
    while(message.at(i) != *","){
        out[j] = message.at(i);
        j++;
        i++;
    }
    
    int output = atoi(out);
    return output;
}

int message::parseOutPort(string message){
    int i = 0;
    char out[15] = {'\0'};
    
    while(message.at(i) != *"\n"){
        i++;
    }
    i+5;
    int j = 0;
    while(message.at(i) != *","){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        out[j] = message.at(i);
        j++;
        i++;
    }
    
    int output = atoi(out);
    return output;
}

void printData(stringInput){
    //to be finished later, will literall skip header part and print whatever remains.
}