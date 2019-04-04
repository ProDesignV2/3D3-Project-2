#include <iostream>
#include <ctime>
#include <fstream>
#include <string> 
#include <cstdlib>
#include <bits/stdc++.h> 

using namespace std;
  
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
struct Graph* createGraph(int V, int E) { 
    struct Graph* graph = new Graph; 
    graph->V = V; 
    graph->E = E; 
    graph->edge = new Edge[E]; 
    return graph; 
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
    cout<<dt<<endl; 
} 

void printArr1(int dist[], int n, int path[], int src, string data[]) { 
    //char letter;
    int V;
    int k;
    time_t current_time=time(0);
    char* dt = ctime(&current_time);
    ofstream myfile;
    ofstream myfile1;
    myfile1.open ("paths.txt",ios::app);
    myfile.open ("nodes.txt", ios::app);
    printf("Vertex   Distance from Source\n");
    myfile << "Vertex   Distance from Source\n"; 
    
    for (int i = 0; i < n; ++i){
        
      // if(dist[i]<10000){
            k=i+65; 
            char letter = k; 
            printf("%c", letter);
            data[i]=letter;
            myfile << letter;
             if(i==src){
                data[i]=data[i]+letter;
            }
            //printf("->");
            if(i!=src){
                printf("->");
                myfile << "->";
                int k=path[i];
            
                if(k==src){
                    int src1=src+65;
                    printf("%c", src1);
                    myfile << (char)(src1);
                    data[i]=data[i]+ (char)(src1);
                }
           
                while(k!=src){
                    printf("%c", k+65);
                    myfile << (char)(k+65);
                    //data[i]=data[i]+ (char)(k+65); // this line gives you the path .......
                    printf("->");
                    myfile << "->";
                    //cout << k;
                    //cout << src;
                
                    k=path[k];
                    if(k==src){
                      printf("%c", k+65);
                      myfile << (char)(k+65);
                      data[i]=data[i]+ (char)(k+65);
                      k=src;
                      break;
                    }

                }

            }
       
            printf("\t\t\t %d\n", dist[i]);
            myfile << "\t\t\t" << dist[i] << "\n";
            data[i]=data[i]+ ",";
            data[i]=data[i] + to_string(dist[i]);
            data[i]=data[i]+ "\n";
            //myfile1<<'|' << endl;
            myfile1<<data[i];
       // }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Need to call function here to get data if u want it
        
    }
    
    cout<<dt<<endl;
    myfile << dt << "\n";
    myfile.close();
    myfile1.close(); 

}

void BellmanFord(struct Graph* graph, int src, string data[]) { 
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

            
            
          
            printArr(dist, V,path,src); 
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

void editdata6(struct Graph* graph, char source, char destination, int weight,string data[]){

    int V = graph->V; 
    int E = graph->E; 
    ofstream inFile;
    inFile.open("paths.txt");
    inFile <<"";
    inFile.close();

    for(int i=0; i<E; i++){
        cout<< graph->edge[i].src<<endl;
        cout<< destination << endl;
        if((graph->edge[i].src)==source && (graph->edge[i].dest)== destination){
            graph->edge[i].weight = weight;
            
        }

        if((graph->edge[i].dest)==source && (graph->edge[i].src)== destination){
            graph->edge[i].weight = weight;
        }

    }

    for(int i=0; i<6; i++){
        BellmanFord(graph, i,data);
     
    }

}

void editdata7(struct Graph* graph, char source, char destination, int weight,string data[]){

    int V = graph->V; 
    int E = graph->E; 
    ofstream inFile10;
    inFile10.open("paths.txt");
    inFile10 <<"";
    inFile10.close();

    for(int i=0; i<E; i++){
        cout<< graph->edge[i].src<<endl;
        cout<< destination << endl;
        if((graph->edge[i].src)==source && (graph->edge[i].dest)== destination){
            graph->edge[i].weight = weight;
            
        }

        if((graph->edge[i].dest)==source && (graph->edge[i].src)== destination){
            graph->edge[i].weight = weight;
        }

    }

    for(int i=0; i<7; i++){
        BellmanFord(graph, i,data);
     
    }

}

int main() { 
///////////////////////////////////////////////////////////////////////this needs to go in main file
    string data[7];
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
        cout << "File has been opened" << endl;
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
        cout << "File has been opened" << endl;
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

    editdata7(graph, 'A','B', 1,data); //this leads to bellanford which leads to printarray and printarray1 and top 3 functions and all the lib aswell
    editdata7(graph, 'A','E', 3,data);
    //editdata6(graph, 'C','B', 2,data);
    //editdata6(graph, 'D','B', 4,data);

    return 0;
}