#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "header.h"
#include <stack>
#include <iostream>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(){
    double a = 1.0,b = 100.0,eps = 1e-6 ,s =12.0;
    queue<double*> globQueue;
    double *Interval =new double[2] {a, b};
    globQueue.push(Interval);

    double M = max(f(a),f(b));
    //Set initialized size for the queue
    int initialSize = 1000;


    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    //Begin generating many intervals
    while (globQueue.size() < initialSize){
	int Qsize = globQueue.size();

	#pragma omp parallel for schedule(dynamic)
        for (int i =0; i< Qsize; i++){
	    double *currInterval;
	    #pragma omp critical
	    {
	        currInterval = globQueue.front();
	        globQueue.pop();
	    }
	    double currL = currInterval[0], currR = currInterval[1];
	    double lvalue,rvalue,posM, res[3];
	    posM = find_Max(currL, currR, s, res);
	    lvalue = res[0]; rvalue =res[1];
	    if (posM > M + eps){
		double *left = new double[2] {currL, (currL + currR)/2};
		double *right = new double[2] {(currL + currR)/2, currR};
//		globQueue.push(left);
//		globQueue.push(right);
		#pragma omp critical
		{
		globQueue.push(left);
		globQueue.push(right);
		M = max(max(M, lvalue), rvalue);		
		}
	    }
        }
    }
    cout<< " The Queue is already initializedi with "<<globQueue.size() << " intervals" << endl;
    //create a global stack for all threads to use
//    stack<double*> globStack;

    int Qsize = globQueue.size();
    //Assign tasks to threads
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i< Qsize; i++){
        double *currInterval;
	stack<double*> globStack;
	#pragma omp critical
	{
	currInterval = globQueue.front();
	globQueue.pop();
	}
	double currL = currInterval[0], currR = currInterval[1];
	double init_interval[2] = {currL, currR};
	globStack.push(init_interval);
	double lvalue,rvalue,posM,res[3];
	double left[2],right[2];

	while (!globStack.empty()){
//	    #pragma omp critical
//	    {
	    currInterval = globStack.top();
	    globStack.pop();
//	    }
	    currL = currInterval[0], currR = currInterval[1];
	    posM = find_Max(currL, currR, s, res);
	    lvalue = res[0]; rvalue = res[1];
	    if (posM > M + eps){
	        left[0] = currL; left[1]= (currL + currR)/2;
	        right[0] = (currL + currR)/2; right[1] =  currR;
	        globStack.push(left);
	        globStack.push(right);
		#pragma omp critical
		{
//	        globStack.push(left);
//	        globStack.push(right);
		M = max(max(M, lvalue), rvalue);
		}
	    }
	}
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2 - t1).count();
    cout << "Program lasts "<< duration <<" seconds."<< endl;
    cout << "The Final result is :" << setprecision(15)  << M << endl;
}
