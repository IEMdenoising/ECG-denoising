#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <complex>
#include <iterator>
#include <fstream>
#include <Eigen>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <list>
#include <tuple>
#include <math.h>    
#include <algorithm>    // std::sort


using Eigen::VectorXd;

/*
 Filters the input data, x, using a rational transfer function defined by the numerator and denominator coefficients A and B, respectively.
*/

VectorXd IIRfilter(vector<double> A,vector<double> B, VectorXd signal){
	
	VectorXd noisy_imfs(signal.size());
	int sizeb=B.size();
    vector<double> X;
    vector<double> filt;			    //filtered signal
    int init=sizeb+1;				
    double sumx;						//sum of x[...]*b[...]
    double sumy;						//sum of y[...]*a[...]
	
	for(int i=0;i<sizeb;i++){
		X.push_back(0);				    //append signal by zeros
	}	
	for(int i=0;i<signal.size();i++){
		X.push_back(signal[i]);
	}
	
	for(int i=0;i<X.size()-sizeb;i++){
		filt.push_back(0);			    //filtred signal filled by zeros
	}
	
	
	for(int i=sizeb;i<X.size()-1;i++){  // filtration
		sumx=B[0]*X[i];
		sumy=0;
		
		for(int j=1;j<sizeb;j++){
			sumx=B[j]*X[i-j]+sumx;
			
		}
		
		for(int j=1;j<A.size();j++){
			sumy=A[j]*filt[i-j]+sumy;
			
		}
		
		filt[i]=(sumx-sumy)/A[0];
		
	}
	
		for(int i=0;i<sizeb;i++){
		
		filt.erase(filt.begin());       //erase zeros from filtered signal
		
	}
	
	for(int i=0;i<noisy_imfs.size();i++){
	noisy_imfs[i]=filt[i];
}
	return noisy_imfs;
	
}




// Wojciech Kot 2016








