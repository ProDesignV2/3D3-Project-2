#ifndef BELLMAN_HPP
#define BELLMAN_HPP

#include <string>

#define INFOAMT 14
#define M 7
#define NODEAMT 8
#define BUFFER_SIZE 1024

// Bellman-Ford algorithm header file

// a structure to represent a weighted edge in graph 
struct Edge { 
    int src, dest; 
    int weight; 
};   

// a structure to represent a connected, directed and  
// weighted graph 
struct Graph { 
    // V-> Number of vertices, E-> Number of edges 
    int V, E; 
    // graph is represented as an array of edges. 
    struct Edge* edge; 
};  

// Creates a graph with V vertices and E edges 
struct Graph* createGraph(int V, int E);

void printArr(int dist[], int n, int path[], int src);

void printArr1(int dist[], int n, int path[], int src, std::string data[]);

void BellmanFord(struct Graph* graph, int src, std::string data[], int overallpath[M][M]);

void editdata6(struct Graph* graph, char source, char destination, int weight, std::string data[]);

void editdata7(struct Graph* graph, char source, char destination, int weight, std::string data[], bool newDV,int overallpath[M][M]);

int getnextNode(char dest,char source, int overallpath[M][M])

struct Graph* bellmanSetup();

void bellmanUpdateFile(struct Graph* graph, std::string **dv, int num_dv);

bool bellmanUpdateArray(std::string nodeAndPort[NODEAMT][INFOAMT]);

void bellmanSetupFile(struct Graph* graph, std::string nodeAndPort[NODEAMT][INFOAMT], char present[NODEAMT]);

#endif
