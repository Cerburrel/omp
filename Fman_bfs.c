#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "header.h"
#include <stack>
#include <iostream>
using namespace std;

int main(){
    double a = 1.0,b = 100.0,eps = 1e-6 ,s =12.0;
    queue<double*> globQueue;
    double *Interval =new double[2] {a, b};
    globQueue.push(Interval);

    double M = max(f(a),f(b));
    //Set initialized size for the queue
    int initialSize = 1000;

    //Begin generating many intervals
    while (globQueue.size() < initialSize){
	int Qsize = globQueue.size();
        cout<< "\nThe queue size is: " << Qsize <<endl;
	if (globQueue.empty()){break;}

	#pragma omp parallel for schedule(dynamic)
        for (int i =0; i< Qsize; i++){
//           cout<< "The current max is :"<< M <<endl; 
//           cout<<"Current in Iter "<< i <<endl;
	    double *currInterval;
	    #pragma omp critical
	    {
	        currInterval = globQueue.front();
	        globQueue.pop();
	    }
	    double currL = currInterval[0], currR = currInterval[1];
	    double lvalue,rvalue,posM = find_Max(currL, currR, s);
//	    cout<< "Working on "<< currL<< " and " << currR<< endl;
//	    cout<< "posM is "<< posM<< endl;
	    if (posM > M + eps){
		double *left = new double[2] {currL, (currL + currR)/2};
		double *right = new double[2] {(currL + currR)/2, currR};
		#pragma omp critical
		{
		globQueue.push(left);
		globQueue.push(right);
		M = max(max(M, lvalue), rvalue);		
		}
//		cout<< "Newly added two intervals, now the queue has :"<< globQueue.size() <<endl;
	    }
        }
    }
    cout<< " The Queue is already initializedi with "<<globQueue.size() << " intervals" << endl;
    //create a global stack for all threads to use
    stack<double*> globStack;

    int Qsize = globQueue.size();
    //Assign tasks to threads
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i< Qsize; i++){
        double *currInterval;
	double currL, currR;
	double lvalue,rvalue,posM;

	while (!globQueue.empty()){
	    //dsj
            #pragma omp critical
	    {
	    currInterval = globQueue.front();
	    globQueue.pop();
	    }
	    currL = currInterval[0], currR = currInterval[1];
	    cout << "currently working on "<< currL<< " and "<< currR <<endl;
	    cout << "The current max is :" << M <<endl;
	    cout << "There are " << globQueue.size() << "intervals remaining" << endl;
	    lvalue,rvalue,posM = find_Max(currL, currR, s);
	    if (posM > M + eps){
	        double *left = new double[2] {currL, (currL + currR)/2};
	        double *right = new double[2] {(currL + currR)/2, currR};
		#pragma omp critial
		{
	        globQueue.push(right);
	        globQueue.push(left);
		M = max(max(M, lvalue), rvalue);
		}
	    }
	}
    }
    
    cout << "The Final result is :" << M << endl;
}
