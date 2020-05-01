#include <iostream>

#include <thread>

#include <iomanip>

#include <vector>

#include <string>

using namespace std;

//Declare FactorsofNumber as type

//of vector<int>

vector<int>FactorsofNumber;

//Implementation of sumofFactors function

void sumofFactors(int from, int to, int N)

{   

     //Declare k as integer type

     int k;

     k = from;

     //Iterate the loop

     while (k <= to)

     {

          //check N%k is equal to 0 or not

          if (N%k == 0)

          {

              FactorsofNumber.push_back(k);

          }

          //increment k by 1

          k = k+1;

     }

}

//Implementation of main function with argumenttotal as

//type of integer and argumentVector as type of char **

int main(int argumenttotal, char **argumentVector)

{

     //Declare i,k,j, N,P,total, halfofThreads, factorsSum

     //as type of integer

     int i, k, j, N, P, total,halfofThreads, factorsSum;

     //checl argumenttotal is total greater than

     //equal to 2

     if (argumenttotal >= 2)

     {

          //Get the N from command line

          N= stoi(argumentVector[1]);

          //Get the P from command line

          P = stoi(argumentVector[2]);

     }

     halfofThreads = P / 2;

     int differentThreads = halfofThreads / P;

     //initailize total with 0

     total = 0;

     //creating number of threads

     thread numberofThread[P];

     //initialize k with 0

     k = 0;

     //Iterate the loop

     while (k < P - 1)

     {

          //assign resultant value of thread function to numberofThread[k]

          numberofThread[k] = thread(sumofFactors, total*differentThreads + 1,

                                  (total + 1)*differentThreads, N);

          //increment total by 1

          total = total + 1;

          //increment k by 1

          k = k+1;

     }

     //assign resultant value of thread function to numberofThread[p-1]

     numberofThread[P - 1] = thread(sumofFactors, total*differentThreads + 1,

                   halfofThreads, N);

     //initialize i with 0

     i = 0;

     //Iterate the loop

     while (i < P)

     {

          //join the threads

          numberofThread[i].join();

          //increment by 1

          i++;

     }

     //initialize factorsSum with 0

     factorsSum = 0;

     //initialize j with 0

     j = 0;

     //Iterate the loop

     while (j < FactorsofNumber.size())

     {

          //calculate the factorsSum

          factorsSum = factorsSum + FactorsofNumber[j];

          //increment j by 1

          j = j+1;

     }

     //check factorsSum is equal to N

     //then it is perfect number

     if (factorsSum == N)

     {

          //Display statement

          cout <<"True";

     }

     //check factorsSum is not equal to N

     //then it is not perfect number

     else if (factorsSum != N)

     {

          //Display statement

          cout <<"False";

     }

     return 0;

}