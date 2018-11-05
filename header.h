#include <math.h>
using namespace std;
double f(double x){
    double res = 0.0;
    double res_j = 0.0;
    for (int i = 100; i>0; i--){
        res_j = 0.0;
	for (int j = i; j>0; j--){
	    res_j += pow(x + 0.5 * j, -3.3);
	}
	res += sin(x + res_j)/pow(1.3, i);
    }
    return res;
}

double find_Max(double a, double b, double s,double res[3] ){
    res[0] = f(a);
    res[1] = f(b);
    res[2] = (res[0] + res[1] + s*(b-a))* 0.5;
    res[2] = max(max(res[2], res[1]),res[0]);
    return res[2];
}
