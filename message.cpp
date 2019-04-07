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

//Added timestamp
string message::createDataHeader(char *source, char *dest, int rport, int sport, string data){
    string timeOfDay;
    time_t current_time = time(0);
    timeOfDay = ctime(&current_time);

    int pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1, 8);


    string out = "Data\n";
    out.append(timeOfDay);
    out += "\n";
    out.append(source); 
    out += ",";
    out.append(dest);
    out += to_string(rport) + ", " + to_string(sport) + "\n";
    out += to_string(data.length());
    out += "\n\n\r";
    out += data;

    return out;
}

//Added timestamp
string message::createControlHeader(char *source, string array[], int size, char present[HEIGHT], char disFlag){
    //Needed to alter to deal with string inputs
    /*
    int i = 0;
    string out = "Control\n";
    out.append(source);
    out += "\n";
    for(int i = 0; i<size;i++){
        out += to_string(array[i]);
    }
    out += "\n";
    return out;*/


    //type          Control\n
    //timestamp     HH:MM:SS\n
    //costs         A B 6\nA C 4\n.....
    //termination   \n\n\r
    string timeOfDay;
    time_t current_time = time(0);
    timeOfDay = ctime(&current_time);


    int pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1);
    pos = timeOfDay.find(" ");
    timeOfDay = timeOfDay.substr(pos+1, 8);

    string out = "Control\n";
    out.append(source);
    if(disFlag != 0){
        out += " ";
        out += disFlag;
    }
    out += "\n";
    out.append(timeOfDay);
    out += "\n";


    for (int i = 2 ; i < size ; i ++){
        for(int j = 0 ; j < HEIGHT ; j++) {
            if (array[i] != "" && array[i][0] == present[j]) {
                out.append(source);
                out.append(array[i]);
                i++;
                out.append(array[i]);
                out += "\n";
            }
        }
    }

    out += "\n\r";


    return out;
}

//No changes
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

//Newly added
int message::parseTime(string message){
    int i = 0;
    char array[6] = {'\0'};
    int pos = message.find("\n") + 1;
    while(message.at(pos) != *"\n"){
        if(message.at(pos) != ':') {
            array[i] = message.at(pos);
            i++;
        }
        pos++;
    }
    //string output = array;
    return stoi(array);
}

//Updated
char message::parseSource(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;

    out = message.at(i);
}

//Combined together
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
    while(message.at(i) != *"\n"){ //skips "Data\n"
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){ //skips "Time\n"
        i++;
    }
    i++;
    out = message.at(i); //assigns source to output

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

string **message::parseDV(string message, int *num_DVs){
    int i = 0, j = 0, k = 0;
    string ** array;
    array = new string*[HEIGHT];
    for(int h = 0 ; h < HEIGHT ; h++){
        array[h] = new string[3];
        for(int w = 0 ; w < 3 ; w++){
            array[h][w] = "";
        }
    }

    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;

    while(message.at(i) != *"\r") {
        while (message.at(i) != *"\n" ) {
            array[k][j] = message.at(i);
            j++;
            i++;
        }
        i++;
        k++;
        j = 0;
    }

    // Set number of DVs to k - 1
    *num_DVs = k - 1; 

    return array;
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
    i++;
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
    int i = 0;
    int size = input.length();
    while(input.at(i)!= *"\r"){
        i++;
    }
    i++;
    while(i<size){
        cout << input.at(i);
        i++;
    }
}

void message::printHeaderMessage(string input){
    int i = 0;
    while(input.at(i)!= *"\r"){
        cout << input.at(i);
        i++;
    }
}

char message::parseOtherNodes(string** message, int number){
    char out;
    for(int j = 0 ; j < number ; j++){
        out = message[j][1][0];
    }

    return out;
}

char message::passSource(string message){
    int i = 0;
    char out;
    while(message.at(i) != *"\n"){
        i++;
    }
    i++;
    out = message.at(i+2);

    return out;
}