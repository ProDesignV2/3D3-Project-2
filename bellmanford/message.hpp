#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <string>

#define NUM 10

using namespace std;

//I think this should be all functions required for the messages, will add any more 
//that are needed if they arise. 

class message{
    public:
        message();
        //message creation
        string createDataHeader(char *source, char *dest, int rport, int sport, string data); //receive port and send port
        string createControlHeader(char *source, int array[NUM], int size); //type of message, number of connections, string of node names
        //for parsing all messages
        string parseType(string message);
        //for parsing DV sections of control messages
        char parseSource(string message);
        string parseWeights(string message);
        //for parsing data messages
        char parseDataSource(string message);
        char parseDataDest(string message);
        int parseInPort(string message);
        int parseOutPort(string message);
        //funtions to print various messages/payloads
        void printHeaderMessage(string input);
        void printData(string input);

};