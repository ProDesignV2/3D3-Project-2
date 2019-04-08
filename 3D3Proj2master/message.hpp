#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <string>

#define WIDTH 14
#define HEIGHT 8

using namespace std;

//I think this should be all functions required for the messages, will add any more 
//that are needed if they arise. 

class message{
    public:
        message();
        //message creation
        string createDataHeader(char *source, char *dest, int rport, int sport, string data); //receive port and send port
        string createControlHeader(char *source, string array[WIDTH], int size, char present[HEIGHT], char disFlag, char TTL); //type of message, number of connections, string of node names
        //for parsing all messages
        string parseType(string message);
        //for parsing DV sections of control messages
        string parseTime(string message);
        char parseSource(string message);
        string parseWeights(string message);
        //for parsing data messages
        string ** parseDV(string message, int *num_DVs);
        char parseDataSource(string message);
        char parseDataDest(string message);
        int parseInPort(string message);
        int parseOutPort(string message);
        //funtions to print various messages/payloads
        void printHeaderMessage(string input);
        void printData(string input);
        char parseOtherNodes(string** message, int number);
        char passSource(string message);
        char passLength(string message);

};