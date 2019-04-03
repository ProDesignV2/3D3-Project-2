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
    out += to_string(rport) + ", " + to_string(sport) + "\n";
    out += to_string(data.length());
    out += "\n\n\r";
    out += data;

    return out;
}

string message::createControlHeader(char *source, int array[], int size){
    int i = 0;
    string out = "Control\n";
    out.append(source);
    out += "\n";
    for(int i = 0; i<size;i++){
        out += to_string(array[i]);
    }
    out += "\n";
    return out;
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

char message::parseSource(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    out = message.at(i);
}

string message::parseWeights(string message){
    int i = 0;
    int j = 0;
    char out[10] = {'\0'};
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        out[j] = message.at(i);
        j++;
        i++;
    }

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

void message::printData(string input){
    //to be finished later, will literall skip header part and print whatever remains.
}
void message::printHeaderMessage(string input){
    //still to be finished, should be very simple.
}