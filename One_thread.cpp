// C++ Program for Floyd Warshall Algorithm using one thread
//In order to run,use: g++ One_thread.cpp  -std=c++11 -lpthread
//Change the value of V when changing the number of vertices
//Adapted from GeekForGeek
//Author:Yusreen Shah (A00415678)

#include <bits/stdc++.h> 
using namespace std;

// Number of vertices in the graph 
#define V 4

/* Define Infinite as a large enough value */
#define INF 99999 

int **graph = NULL; // matrix that contains the graph
int nodeCount = 0; //number of nodes in the graph
int vertexCount = 0; //number of vertices


// A function to print the solution matrix 
void printSolution(int dist[][V]);

// Solves the all-pairs shortest path 
void floydWarshall(int graph[][V])
{
  /* dist[][] will be the output matrix 
  that will finally have the shortest 
  distances between every pair of vertices */
  int dist[V][V], i, j, k;

  /* Initialize the solution matrix same 
  as input graph matrix. Or we can say 
  the initial values of shortest distances 
  are based on shortest paths considering 
  no intermediate vertex. */
  for (i = 0; i < V; i++)
    for (j = 0; j < V; j++)
      dist[i][j] = graph[i][j];

  /* Add all vertices one by one to 
  the set of intermediate vertices. 
  Before start of an iteration, 
  we have shortest distances between all 
  pairs of vertices such that the 
  shortest distances consider only the 
  vertices in set {0, 1, 2, .. k-1} as 
  intermediate vertices. 
  After the end of an iteration, 
  vertex no. k is added to the set of 
  intermediate vertices and the set becomes {0, 1, 2, .. k} */
  for (k = 0; k < V; k++)
  {
    // Pick all vertices as source one by one 
    for (i = 0; i < V; i++)
    {
      // Pick all vertices as destination for the 
      // above picked source 
      for (j = 0; j < V; j++)
      {
        // If vertex k is on the shortest path from 
        // i to j, then update the value of dist[i][j] 
        if (dist[i][k] + dist[k][j] < dist[i][j])
          dist[i][j] = dist[i][k] + dist[k][j];
      }
    }
  }

  // Print the shortest distance matrix 
  printSolution(dist);
}

/* A utility function to print solution */
void printSolution(int dist[][V])
{
  cout << "The following matrix shows the shortest distances"
    " between every pair of vertices \n";
  for (int i = 0; i < V; i++)
  {
    for (int j = 0; j < V; j++)
    {
      if (dist[i][j] == INF)
        cout << "INF" << "	 ";
      else
        cout << dist[i][j] << "	 ";
    }
    cout << endl;
  }
}

void printGraph()
{
  //Get the number of nodes and the number of undirected edges from the user
  while (true)
  {
    std::cout << std::endl;
    std::cout << 
      "Enter the number of nodes and the number of undirected edges:";
    std::cin >> nodeCount;
    if (nodeCount < 1)
    {
      std::cout << "Sorry! The number of nodes must be greater than 0";
      std::cout << std::endl;
    }
    else
    {
      break;
    }
  }
  std::cout << std::endl;

  //Get number of vertices from the user
  while (true)
  {
    std::cout << "Input the edges here:";
    std::cin >> vertexCount;
    std::cout << std::endl;
    if (vertexCount < 1)
    {
      std::cout << "Sorry! The number of vertices must be greater than 0";
      std::cout << std::endl;
    }
    else
    {
      break;
    }
  }
  //Allocate the initial graph
  graph = (int **)malloc(sizeof(int*) * nodeCount);
  for (int i = 0; i < nodeCount; i++)
  {
    graph[i] = (int*)malloc(sizeof(int) *nodeCount);
  }

  //Get the vertexCount
  for (int i = 0; i < nodeCount; i++)
  {
    for (int j = 0; j < nodeCount; j++)
    {
      if (i == j)
      {
        graph[i][j] = 0;
      }
      else
      {
        graph[i][j] = INF;
      }
    }
  }
  int *vertex = (int*)malloc(sizeof(int) *3);
  for (int i = 0; i < vertexCount; i++)
  {
    std::cout << "Please enter the edge and the weight of that edge (" << i + 1
      << ") : ";
    std::cin >> vertex[0];
    std::cin >> vertex[1];
    std::cin >> vertex[2];
    std::cout << std::endl;
    if (vertex[2] < 0)
    {
      std::cout << "Sorry! You cannot provide a negative edge weight.";
      std::cout << std::endl;
      i--;
    }
    graph[vertex[0] - 1][vertex[1] - 1] = vertex[2];
    graph[vertex[1] - 1][vertex[0] - 1] = vertex[2];
  }
} // end of printGraph

// Driver code 
int main()
{
  printGraph();

  //Print the initial matrix
  std::cout << "\nThis is the initial matrix:\n\n";
  for (int i = 0; i < nodeCount; i++)
  {
    for (int j = 0; j < nodeCount; j++)
    {
      if (graph[i][j] == INF)
      {
        std::cout << "INF "; //print INF
      }
      else
      {
        std::cout << graph[i][j] << " "; //print the graph
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  clock_t start, end;


  // Print the solution 
  int inputGraph[V][V];
  for (int i = 0; i < nodeCount; i++)
  {
    for (int j = 0; j < nodeCount; j++)
    {
      inputGraph[i][j] = graph[i][j];
    }

  }

  // Recording the starting clock tick.//
  start = clock();
  floydWarshall(inputGraph);
  // Recording the ending clock tick.//
  end = clock();
  // Calculating total time taken by the program. 
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  std::cout << "Time taken by program is : " << time_taken;
  std::cout << " sec " << std::endl;
  return 0;
}