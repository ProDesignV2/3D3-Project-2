#include <iostream>
#include <ctime>
#include <fstream>
#include <string> 
#include <cstdlib>
#include <bits/stdc++.h> 

#include "bellman.hpp"
#include "message.hpp"

using namespace std;
const int M = 7; 

// Creates a graph with V vertices and E edges 
struct Graph* createGraph(int V, int E) { 
    struct Graph* graph = new Graph; 
    graph->V = V; 
    graph->E = E; 
    graph->edge = new Edge[E]; 
    return graph; 
}


int getnextNode(char dest,char source, int overallpath[M][M]){

    int next_node; 

    next_node=overallpath[source][dest]; 
    cout << next_node << endl;

    next_node=next_node+ 10000 ;

    return next_node;


}

void printArr(int dist[], int n, int path[], int src) { 
    //char letter;
    int V;
    int k;
    time_t current_time=time(0);
    char* dt = ctime(&current_time);
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i){
        if(dist[i]<1000){
        k=i+65;
        char letter = k; 
        printf("%c", letter);
        //printf("->");
        if(i!=src){
            printf("->");
            int k=path[i];
            
            if(k==src){
                int src1=src+65;
                printf("%c", src1);
            }
           
            while(k!=src){
                printf("%c", k+65);
                printf("->");
                //cout << k;
                //cout << src;
                
                 k=path[k];
                 if(k==src){
                      printf("%c", k+65);
                      k=src;
                      break;
                 }

            }

        }
        printf("\t\t\t %d\n", dist[i]);
        }
    }
    // cout<<dt<<endl; 
} 

void printArr1(int dist[], int n, int path[], int src, string data[]) { 
    //char letter;
    int V;
    int k;
    time_t current_time=time(0);
    char* dt = ctime(&current_time);
    // ofstream myfile;
    ofstream myfile1;
    myfile1.open ("paths.txt",ios::app);
    // myfile.open ("nodes.txt", ios::app);
    // printf("Vertex   Distance from Source\n");
    // myfile << "Vertex   Distance from Source\n"; 
    
    string temp = "";

    for (int i = 0; i < n; ++i){
        
      // if(dist[i]<10000){
            k=i+65; 
            char letter = k; 
            // printf("%c", letter);
            data[i]=letter;
            // myfile << letter;
             if(i==src){
                data[i]=data[i]+letter;
            }
            //printf("->");
            if(i!=src){
                // printf("->");
                // myfile << "->";
                int k=path[i];
            
                if(k==src){
                    int src1=src+65;
                    // printf("%c", src1);
                    // myfile << (char)(src1);
                    data[i]=data[i]+ (char)(src1);
                }
           
                while(k!=src){
                    // printf("%c", k+65);
                    // myfile << (char)(k+65);
                    //data[i]=data[i]+ (char)(k+65); // this line gives you the path .......
                    
                    // char temp_char = (char)(k+65);
                    // temp.append(to_string(temp_char));

                    // printf("->");
                    // myfile << "->";
                    cout << k;
                    //cout << src;
                
                    k=path[k];
                    if(k==src){
                      // printf("%c", k+65);
                    //   myfile << (char)(k+65);
                      data[i]=data[i]+ (char)(k+65);
                      k=src;
                      break;
                    }

                }

            }
       
            // printf("\t\t\t %d\n", dist[i]);
            // myfile << "\t\t\t" << dist[i] << "\n";
            data[i]=data[i]+ ",";
            data[i]=data[i] + to_string(dist[i]);
            data[i]=data[i]+ "\n";
            //myfile1<<'|' << endl;
            // cout << temp << endl;
            myfile1<<data[i];
       // }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Need to call function here to get data if u want it
        
    }
    
    // cout<<dt<<endl;
    // myfile << dt << "\n";
    // myfile.close();
    myfile1.close(); 

}

void BellmanFord(struct Graph* graph, int src, string data[],int overallpath[M][M]) { 
    int V = graph->V; 
    int E = graph->E; 
    int max=0;
    int dist[V];
    int path[V]; 
    int overallpath[V][V];
    

  
    // Step 1: Initialize distances from src to all other vertices 
    // as INFINITE 
    for (int i = 0; i < V; i++){ 
        dist[i]   = INT_MAX; 
        dist[src] = 0;
        path[i] = src;
        
    } 
        path[src] = src;
        path[V]=99;
  
    // Step 2: Relax all edges |V| - 1 times. A simple shortest  
    // path from src to any other vertex can have at-most |V| - 1  
    // edges
    int t=1; 
    for (int i = 1; i <= V-1; i++) { 
        for (int j = 0; j < E; j++) { 
            
            int u = (graph->edge[j].src)-65; 
            int v = (graph->edge[j].dest)-65; 
            int weight = graph->edge[j].weight;

            if(weight>=0){ 
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]){ 
                    dist[v] = dist[u] + weight;
                    if(v!=src){
                    path[v] = u;
                    }
                    
              
               
                }

            
            
          
            // printArr(dist, V,path,src); 
            } 
         
        } 

      
        for(int i=0; i<V; i++){
            //cout<< path[i] << " ";
            overallpath[src][i]=path[i];
            if (path[i]>max&&path[i]<99){
                max=path[i];//need to fix max path
                //cout<<max;
            }
            //cout<<endl;
        }
    }
    
    printArr1(dist,7,path, src,data); 
    //cout<<"yeppp"<<endl;
    return; 
}



void editdata7(struct Graph* graph, char source, char destination, int weight, string data[], bool newDV,int overallpath[M][M]){

    int V = graph->V; 
    int E = graph->E; 
    ofstream inFile10;
    inFile10.open("paths.txt");
    inFile10 <<"";
    inFile10.close();

    // Change link costs if new DVs
    if(newDV){
        for(int i=0; i<E; i++){
            // cout<< graph->edge[i].src<<endl;
            // cout<< destination << endl;
            if((graph->edge[i].src)==source && (graph->edge[i].dest)== destination){
                graph->edge[i].weight = weight;
                
            }

            if((graph->edge[i].dest)==source && (graph->edge[i].src)== destination){
                graph->edge[i].weight = weight;
            }
        }
    }

    for(int i=0; i<7; i++){
        BellmanFord(graph, i, data, overallpath);
    }

}

struct Graph* bellmanSetup(){

    ifstream inFile;
    string links,answer,answer2;
    int answer1,deleted; 
    char node1,node2,weight,delete1;  
    int no_edges=0;
    ofstream myfile2;
    myfile2.open ("paths.txt");
    myfile2<<"";
    myfile2.close();

    inFile.open("links.csv");
    if (inFile.is_open()) {
        //cout << "File has been opened" << endl;
    }
    else {
        cout << "NO FILE HAS BEEN OPENED" << endl;
    }
    int k;
    inFile.close();

    /* Let us create the graph given in above example */
    int V = 7;  // Number of vertices in graph
    int E = 42;  // Number of edges in graph 
    struct Graph* graph = createGraph(V, E); 



    inFile.open("links.csv");
    if (inFile.is_open()) {
        //cout << "File has been opened" << endl;
    }   
    else {
        cout << "NO FILE HAS BEEN OPENED" << endl;
    }
    string line;
    string node;
    vector<string> row; 
    int i=0; //this loop creates an empty graph
    while (!inFile.eof()) {

        getline(inFile, line); 
        //cout<<line<< endl;
    
        int n = line.length(); 
        // declaring character array 
        char char_array[n + 1]; 
  
        // copying the contents of the 
        // string to char array 
        strcpy(char_array, line.c_str()); 

        int k1 = (char_array[0])-65;
        //cout<< k1<<endl;
        if(k1<0){
        inFile.close();
        break;
        }
        int k2 = (char_array[2])-65;
        int k3 = (char_array[4])-100;

        if(k1>=0){
            graph->edge[i].src = char_array[0]; 
            graph->edge[i].dest = char_array[2]; 
            graph->edge[i].weight = k3; 
            //cout << graph->edge[i].weight<< endl;
        
            i++;
        
        }
    }

    return graph;
}

void bellmanUpdateFile(struct Graph* graph, string **dv, int num_dv){ 

    string data[7];
    
    // If no new DV, just run Bellman-Ford algorithm on current graph
    if(num_dv == 0){ editdata7(graph, 0, 0, 0, data, false); return; }

    // Update graph and store paths in file, for each DV e.g. [A B 7]
    for(int i = 0; i < num_dv; i++){
        editdata7(graph, dv[i][0][0],dv[i][1][0], stoi(dv[i][2]), data, true);
    }

    //editdata7(graph, 'A','B', 1, data); //this leads to bellanford which leads to printarray and printarray1 and top 3 functions and all the lib aswell
    //editdata7(graph, 'A','E', 3, data);
    //editdata6(graph, 'C','B', 2,data);
    //editdata6(graph, 'D','B', 4,data);
}

bool bellmanUpdateArray(string nodeAndPort[NODEAMT][INFOAMT]){
    // Load "paths.txt" file into 2D array
    ifstream inFile;
    inFile.open("paths.txt");
    if (inFile.is_open()){
        //cout << "File has been opened" << endl;
    }
    else{ cout << "NO FILE HAS BEEN OPENED" << endl; }
    string line;
    bool updated = false;
    while (!inFile.eof()) {
        // Get DV line e.g. [DC,3]
        getline(inFile, line);
        if(line[0] < 32){ break; }
        // Skip current line if link to self e.g. [BB,4]
        if(line[0] == line[1]){ continue; }
        // Skip current line if link cost is INFINITY or 0
        int link_cost = stoi(line.substr(3));
        if(link_cost == 0 || link_cost == INT_MAX){ continue; }
        // For current line, say [AB,5], check array for source row 'B'
        for(int source = 0; source < NODEAMT; source++){
            if(line[1] == nodeAndPort[source][0][0]){
                // For current source ('B') check row for destination ('A')
                for(int dest = 2; dest < INFOAMT; dest += 2){
                    if(nodeAndPort[source][dest].empty()){
                        // Add path to nodeAndPort if no previous link cost
                        nodeAndPort[source][dest] = line[0];
                        nodeAndPort[source][dest + 1] = to_string(link_cost);
                        // Flag updated link cost
                        updated = true;
                        // Finish looping through neighbours as rest are empty
                        break;
                    }
                    else if(line[0] == nodeAndPort[source][dest][0]){
                        // Update nodeAndPort if lower link cost
                        int curr_link_cost = stoi(nodeAndPort[source][dest + 1]);
                        if(link_cost < curr_link_cost){ 
                            nodeAndPort[source][dest + 1] = to_string(link_cost); 
                            // Flag updated link cost
                            updated = true;
                        }
                        // Finish looping as neighbour found
                        break;
                    }
                }
            }
        }
    }
    // Close "paths.txt"
    inFile.close();
    // Return true if updated, false otherwise
    return updated;
}

void bellmanSetupFile(struct Graph* graph, string nodeAndPort[NODEAMT][INFOAMT], char present[NODEAMT]){



    for(int source = 0; source < NODEAMT; source++){

        char buf[BUFFER_SIZE];
        message DistanceVector;
        char source_name = nodeAndPort[source][0][0];

        //if(source_name == 0){ cout << "Source [" << source << "]" << endl; continue; }

        // Reset buffer
        memset(&buf, 0, sizeof buf);
        // Put DV into buffer
        strcpy(buf, (DistanceVector.createControlHeader(&source_name, nodeAndPort[source], INFOAMT, present, 0, '0')).c_str());
        
        // Counter for DV lines
        int num_DVs = 0;
        // Parse DV lines from buffer
        std::string **DV = DistanceVector.parseDV(buf, &num_DVs);

        bellmanUpdateFile(graph, DV, num_DVs);
    }
}
