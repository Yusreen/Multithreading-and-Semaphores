// C++ Program for Floyd Warshall Algorithm using multiple threads
//In order to run,use: g++ Mul_thread.cpp  -std=c++11 -lpthread
//Author:Yusreen Shah (A00415678)
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <iostream>
#include <semaphore.h>
#include <thread>
#include <mutex>
#include <bits/stdc++.h> 

//Defining the variables
int INF = INT_MAX; // maximum number for infinity value
//Global variables
int **graph = NULL; // matrix that contains the graph
int **dist = NULL; //distance between each node
int numOfNodes = 0; //number of nodes in the graph

int readCount = 0; //the read count
mutex readCountLock; //read count lock
sem_t semphoreRW; //a semaphore for reading and writing


//Define the vertices
struct vertex{ 
    int i;
    int k;
	
    };
	
/**
This method gets the initial matrix (graph) from the user
 */
 void printGraph()
 {
	//Get the number of nodes and the number of undirected edges from the user
   while(true){
	    cout << endl;
        cout << "Enter the number of nodes and the number of undirected edges:";
        cin >> numOfNodes;
        if (numOfNodes < 1 ){
        cout << "Sorry! The number of nodes must be greater than 0";
        cout << endl;
        }
        else {
            break;
        }
    }
	cout << endl;
   int numOfVertices = 0; //number of vertices in graph
   //Get number of vertices from the user
   while(true){ 
      cout << "Input the edges here:";
      cin >> numOfVertices;
		cout << endl;
        if (numOfVertices < 1 ){
        cout << "Sorry! The number of vertices must be greater than 0";
           cout << endl;
        }
        else {
            break;
        }
    }
	//Allocate the initial graph
   graph = (int**)malloc(sizeof (int*) * numOfNodes);
   for (int i = 0 ; i < numOfNodes ; i++){
	   //Create a row
       graph[i] = (int*) malloc(sizeof (int) *numOfNodes);
   }
   
  //Add values to initial graph [contains 0,weight and INF]
   for (int i = 0 ; i < numOfNodes ; i ++){
       for (int j = 0 ; j < numOfNodes ;j ++){
		   //add numbers in each row
           if (i == j ){
               graph[i][j] = 0;
           }
           else {
               graph[i][j] = INF;
           }
       }
   }
   //Create an array for each set of edge and weight
    int* vertex = (int*)malloc(sizeof (int) * 3 );
   for (int i = 0 ; i < numOfVertices ; i++){
     cout << "Please enter the edge and the weight of that edge (" << i+1 << ") : ";
    cin >> vertex[0];
      cin >> vertex[1];
       cin >> vertex[2];//weight of edge
      cout << endl;
       if (vertex[2] < 0){
        cout << "Sorry! You cannot provide a negative edge weight.";
		   cout << endl;
           i--;
       }
	   //Add weight to the graph for each edges
	   //e.g if input is 1 2 1, put 1 in graph[0][1]
       graph[vertex[0]-1][vertex[1]-1] = vertex[2];
	   //We note that this is circular, so put 1 in graph[1][0]
	   //as well
       graph[vertex[1]-1][vertex[0]-1] = vertex[2];
	  
   }
   //Delete vertex array
   delete[] vertex;
  
 }// end of printGraph
 
 /**
 This method is carried by each worker thread.
 */
 
 void worker (vertex data)
 {
	int i = data.i;
    int k = data.k;
	//Read the data
	 for (int j = 0 ; j < numOfNodes;  j++) {
       readCountLock.lock(); //acquire Read Lock so that no other threads can read
       readCount++; 
       if (readCount == 1){ //verifying if first thread is available to be read
           sem_wait(&semphoreRW); //stop any other threads from writing
       }
       readCountLock.unlock(); //let other threads to read 
	   
       if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
	   {
           {
               //Start reading
               readCountLock.lock();
               readCount--;
               if (readCount == 0 )
			   {
				   //acquire write
                   sem_post(&semphoreRW);
               }
               readCountLock.unlock();
           }
		  
           sem_wait(&semphoreRW); 
           dist[i][j] = dist[i][k] + dist[k][j];
           sem_post(&semphoreRW);//write
       }
       else {
           //Start reading
           readCountLock.lock(); //lock the read count lock
           readCount--; 
           if (readCount == 0 ){
			   //acquire write
               sem_post(&semphoreRW); 
           }
           readCountLock.unlock(); //unlock read count lock
       }
    }// end of for loop
 }//end of worker method
 
 /**
 This is the method for the FloydWarshall
 */
 void floydWarshall(){ 
   thread* threadW = (thread*)malloc(sizeof (thread) * numOfNodes); //create thread
	
    for (int k = 0 ; k < numOfNodes;  k++) {
        for (int i = 0 ; i < numOfNodes;  i++){
            vertex data; //get object
			//assign attributes
            data.i = i; 
            data.k = k;
            threadW[i] =move(thread(worker,data));
        }
        for (int thread= 0; thread < numOfNodes ; thread++){
            threadW[thread].join();//join the threads
        }
    }
}
//Main driver
int main() 
{
	//call the printGraph method
    printGraph(); 
    sem_init(&semphoreRW,false, 1); //initialize semaphore
	clock_t start, end; 
	
    //Allocate the distance
    dist = (int**)malloc(sizeof (int*) * numOfNodes);
    for (int i = 0 ; i < numOfNodes; i++){
        dist[i] = (int*) malloc(sizeof (int) *numOfNodes);
    }
    for (int i = 0 ; i < numOfNodes ; i ++){
        for (int j = 0 ; j < numOfNodes; j++){
          dist[i][j] = graph[i][j];
        }
    }
	
	
    // unsync the I/O of C and C++. 
    ios_base::sync_with_stdio(false); 	
    
	// Recording the starting clock tick.//
    start = clock(); 
  //Call floydWarshall() 
    floydWarshall();
	// Recording the ending clock tick.//
    end = clock(); 
  
	
    //Print the initial matrix
   cout << "\nThis is the initial matrix:\n\n";
    for (int i = 0 ; i < numOfNodes; i++){
        for (int j = 0; j < numOfNodes; j++){
			if (graph[i][j] == INF) { 
				cout << "INF "; //print INF
			}
			else {
				cout << graph[i][j] << " "; //print the graph
			}
        }
       cout << endl;
    }
   cout << endl;
	
    //Print the final matrix
    cout << "This is the final matrix:\n\n";
    for (int i = 0 ; i < numOfNodes ; i ++){
        for (int j = 0 ; j < numOfNodes; j++){
            if (dist[i][j]  == INF)   { //check if distance is equal to infinity
               cout <<  "INF "; //print INF
            }
            else {
                cout <<  dist[i][j] << " "; //print the distance
            }
        }
       cout << endl;
		
    }
	 cout << endl;
     // Calculating total time taken by the program. 
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << time_taken; 
    cout << " sec " << endl; 
}
	