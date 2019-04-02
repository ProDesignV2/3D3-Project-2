#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <string>

using namespace std;

//I think this should be all functions required for the messages, will add any more 
//that are needed if they arise. 

class message{
    public:
        message();
        //message creation
        string createDataHeader(char *source, char *dest, int rport, int sport, string data); //receive port and send port
        string createControlHeader(int type); //type of message, number of connections, string of node names
        string appendControl(string input, char *source, char *dest, int port, int weight);
        //for parsing all messages
        string parseType(string message);
        //for parsing control messages
        int parseControlType(string message); 
        //for parsing DV sections of control messages
        char parseSource(string message, int num);
        char parseDest(string message, int num); 
        int parsePort(string message, int num);
        int parseWeight(string message, int num);
        //for parsing data messages
        char parseDataSource(string message);
        char parseDataDest(string message);
        int parseInPort(string message);
        int parseOutPort(string message);

        void printData(string input);

};