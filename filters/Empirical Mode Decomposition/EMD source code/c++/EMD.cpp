// EMD.cpp : Defines the entry point for the console application.
//


#include<chrono>
#include <iostream>
#include <ctime>
#include <ratio>
#include <cstdio>
#include <cstdlib>
#include <complex>
#include <Eigen>
#include <iterator>
#include <fstream>
#include <Dense>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <list>
#include <tuple>
#include <math.h>    
#include <algorithm>    
#include "spline.h"
#include "butterworth.h"
#include "IIRfilter.h"


using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::ArrayXf;
using Eigen::ArrayXd;
using namespace std;


VectorXd Diff(vector<double> signal) {

	/*Differences and Approximate Derivatives- calculates differences
	between adjacent elements of X, along the first array dimension*/

	VectorXd diff(signal.size() - 1);				//vector of differences

	for (int iterator = 1; iterator <= signal.size() - 1; iterator++) {

		double difference = signal[iterator] - signal[iterator - 1];
		diff(iterator - 1) = difference;

	}

	return diff;

}


tuple<vector<double>, vector<double>> Extrema(VectorXd sig) {

	/* Function finds localizations of extrema (maxima and minima) in the given signal.

	signal - analysed signal
	iMin - indexes of minima in the given signal
	iMax - indexes of maxima in the given signal
	*/

	vector<double> signal;
	for (int i = 0; i<sig.size(); i++) {
		signal.push_back(sig[i]);
	}

	int len = signal.size();
	bool cont = false;
	double maxval;
	double minval;
	vector<double> iMax;
	vector<double> iMin;
	vector<double> debs;
	vector<double> fins;
	vector<double> indMax;
	vector<double> indMin;
	vector<double> dZero;
	VectorXd diffSig = Diff(signal);
	VectorXd dSig1(diffSig.size() - 1);
	VectorXd dSig2(diffSig.size() - 1);
	dSig1 = diffSig.segment(0, diffSig.size() - 2);
	dSig2 = diffSig.segment(1, diffSig.size() - 1);

	/* finding extrema based on first derivative crossing-zero points*/

	for (int i = 0; i < dSig1.size(); i++) {

		if ((dSig1[i] * dSig2[i]) <= 0) { //zero crossing point of first derivative

			if (dSig1[i]<0) {
				iMin.push_back(i + 1);    // local minimum
			};
			if (dSig1[i]>0) {
				iMax.push_back(i + 1);    //local maximum
			};

		}

	}


	for (int i = 0; i<diffSig.size(); i++) {

		if (diffSig[i] == 0) {

			cont = true;      // there are zero points in first derivative

		}
	}

	if (cont) {               //searching local extremas foer zero points in first derivative

		for (int i = 0; i<signal.size(); i++) {
			if (signal[i] == 0) {
				// finding indexes of 0 in signal, if found insert 1 to dZero vector
				dZero.push_back(1);
				continue;

			}
			// index of 0 not found, insert 0 to dZero vector
			dZero.push_back(0);
		}

		dZero.insert(dZero.begin(), 0);
		dZero.push_back(0);

		VectorXd diffSig2 = Diff(dZero);   // first derivative

										   //fins, debs- bounds of extremas (indexes) in differented signal

		for (int i = 0; i < diffSig2.size(); i++) {

			if (diffSig2[i] == 1) {

				debs.push_back(i);

			}

			if (diffSig2[i] == -1) {

				fins.push_back(i);

			}

		}

		if (debs[0] == 1) {
			if (debs.size()>1) {
				// remove first elements
				debs.erase(debs.begin());
				fins.erase(fins.begin());
			}
			else {
				//clear vectors
				debs = {};
				fins = {};
			}
		}

		if (!debs.empty()) {
			if (fins.back() == len) {
				if (debs.size()>1) {
					// remove last elements
					debs.pop_back();
					fins.pop_back();
				}
				else {
					//clear vectors
					debs = {};
					fins = {};
				}
			}
		}

		int lc = debs.size();
		// check size of bound vector debs, if <0, there are no additional extremas
		if (lc > 0) {
			for (int i = 0; i<lc; i++) {
				if (diffSig(debs[i] - 1)>0) {
					if (diffSig(fins[i]<0)) {
						// calculate indexes of local maxima	
						maxval = round((fins[i] + debs[i]) / 2);
						indMax.push_back(maxval);
					}
				}
				else {
					if (diffSig(fins[i]>0)) {
						// calculate indexes of local minima
						double minval = round((fins[i] + debs[i]) / 2);
						indMin.push_back(minval);
					}
				}
			}

		}

		if (!indMax.empty()) {
			// insert additional maximas, and sort vector
			iMax.insert(iMax.begin(), indMax.begin(), indMax.end());
			sort(iMax.begin(), iMax.end());
		}

		if (!indMin.empty()) {
			// insert additional miniimas, and sort vector
			iMax.insert(iMin.begin(), indMin.begin(), indMin.end());
			sort(iMin.begin(), iMin.end());
		}

	}


	tuple<vector<double>, vector<double>> extremas;
	extremas = make_tuple(iMin, iMax);
	return extremas;
}

bool isimf(VectorXd signal, VectorXd emin, VectorXd emax) {

	/*maximum difference between the number of
	extrema and the number of zero crossings should be 1*/

	int len = signal.size();
	VectorXd u1(len - 1);
	int extremas = 0;
	int crossingpionts;


	ArrayXd a1(len - 1);
	ArrayXd a2(len - 1);
	for (int i = 0; i<len - 1; i++) {
		a1[i] = signal[i];
		a2[i] = signal[i + 1];
	}



	u1 = (a1*a2);
	for (int i = 0; i<u1.size(); i++) {
		if (u1[i]<0) {
			extremas = extremas + 1;             // number of extremas
		}
	}

	crossingpionts = emin.size() + emax.size();  // number of zero crossing points
	if (abs(extremas - crossingpionts)>1) {
		return false;                            // no imf
	}
	return true;                                 // imf
}


tuple<VectorXd, bool> ExtractMode(VectorXd signal) {

	/*
	Function that calculate the Intrinsic Mode Function of given signal
	based on its extemas using b-spline interpolation */

	int len = signal.size();
	vector<double> vmax;							// values of minima
	vector<double> vmin;							// values of maxima
	vector<double> imax;							// indexes of maxima
	vector<double> imin;							// indexes of minima
	VectorXd emin(len);								// lower envelope (with minima)
	VectorXd emax(len);								// upper envelope (with maxima)
	VectorXd envmean;								// mean value of envelopes
	VectorXd detailnext;                            // vector of details for next iteration
	bool flag = false;
	double sd = numeric_limits<double>::infinity();   //set initial standard deviation value


	while (sd>0.3) {                                  // 0.3 = threshold value

		tuple<vector<double>, vector<double>> extr = Extrema(signal);

		imin = get<0>(extr);
		imax = get<1>(extr);

		for (int i = 0; i<imin.size(); i++) {

			vmin.push_back(signal[imin[i]]);       // fill vector of minima values

		}

		for (int i = 0; i<imax.size(); i++) {

			vmax.push_back(signal[imax[i]]);      // fill vector of maxima values

		}

		if (imax.size() <= 2 || imin.size() <= 2 || vmax.size() <= 2 || vmin.size() <= 2) {
			// break if there is not enough poinsts to calculate spline
			flag = true;
			break;

		}


		tk::spline envmin;                          // initialize spline for minima
		tk::spline envmax;							// initialize spline for maxima

		envmin.set_points(imin, vmin);				// calculate spline for minima
		envmax.set_points(imax, vmax);				// calculate spline for maxima

		for (int i = 0; i<len; i++) {

			emin[i] = envmin(i);

		}
		for (int i = 0; i<len; i++) {

			emax[i] = envmax(i);

		}

		envmean = emax + emin;
		envmean = envmean / 2;

		detailnext = signal - envmean;

		if (isimf(signal, emin, emax)) {

			break;                             // break if found imf

		}


		/* Calculating standard deciation sd. Conversion to Array is necessary to properly use equation of sd */

		ArrayXd detailarr(signal.size());              // detail
		ArrayXd detailnextarr(detailnext.size());	   // details for next iteration

		for (int i = 0; i<signal.size(); i++) {

			detailarr[i] = signal[i];

		}

		for (int i = 0; i<detailnext.size(); i++) {

			detailnextarr[i] = detailnext[i];

		}

		sd = (((detailarr - detailnextarr)*(detailarr - detailnextarr)).sum()) / ((detailarr*detailarr).sum());

		signal = detailnext;

		// clear vector to make it empty for next iteration
		vmin.clear();
		vmax.clear();


	}


	tuple<VectorXd, bool>extractmode;
	extractmode = make_tuple(signal, flag);

	return extractmode;

}


bool ismonothonic(VectorXd signal) {

	/* Returns a boolean value indicating whether or not a vector is monotonic.
	By default, imonothonic returns true for  both monotonic increasing
	and monotonic decreasing vectors. 										*/


	bool ascending = true;                    // function monotonically increasing
	bool descending = true;                   // function monotonically decreasing
	double difference;

	for (int iterator = 1; iterator<signal.size() - 1; iterator++) {

		difference = signal[iterator] - signal[iterator - 1];

		if (difference <= 0) {

			ascending = false;

		}

		if (difference >= 0) {

			descending = false;

		}

	}

	return (ascending | descending);

}


tuple<MatrixXd, VectorXd> EMD(VectorXd signal) {

	/*

	Function that calculates  Intrinsic Mode Functions
	of given signal using Empirical Mode Decomposition.

	signal - signal to decompose

	IMFs - an array of IMFs (each row include one IMF)

	res - residual of the signal (signal minus sum of IMFs)

	*/

	tuple<VectorXd, bool> Extracted;
	VectorXd res = signal;            // set initial value of residue as raw signal
	VectorXd imf;
	MatrixXd IMFs(1, 1);
	MatrixXd IMFstemp;
	int len = signal.size();
	int n = 1;
	bool flag;

	while (~ismonothonic(res)) {      // check if residue is monothonic

		Extracted = ExtractMode(res);

		imf = get<0>(Extracted);
		flag = get<1>(Extracted);     // true if there is no more imfs

		if (flag == true) {
			break;
		}


		IMFs.resize(n, len);

		for (int i = 0; i<IMFstemp.rows(); i++) {

			for (int j = 0; j<IMFstemp.cols(); j++) {

				IMFs(i, j) = IMFstemp(i, j);

			}

		}

		for (int i = 0; i<len; i++) {

			IMFs(n - 1, i) = imf[i];

		}

		IMFstemp.resize(n, len);
		IMFstemp = IMFs;
		n++;

		res = signal - imf;
		signal = res;

	}

	tuple<MatrixXd, VectorXd>EMD;
	EMD = make_tuple(IMFs, res);


	return EMD;

}



VectorXd Denoising(VectorXd signal) {

	/* Denoising using EMD */

	tuple<MatrixXd, VectorXd>decompsig;       // signal decomposited into imfs

	decompsig = EMD(signal);

	MatrixXd IMFs = get<0>(decompsig);        // an array of IMFs (each row include one IMF)
	VectorXd res = get<1>(decompsig);         // residual of the signal (signal minus sum of IMFs)

	VectorXd imf(IMFs.cols());                // vector of single imf

	int num_noised = 3;                       // number of imfs with noise
	VectorXd noisy_imfs(signal.size());       // vector of noisy imf, after filtration filtered imf
	VectorXd sum_imf(signal.size());          // vector of summed imfs

											  // Filter parameters
	vector <double> NumC;                     // numerator coefficients of the rational transfer function
	vector <double> DenC;                     // denominator coefficients of the rational transfer function       
	int FiltOrd = 3;                          // filter order

	double Fs = 360;                          //sampling frequency
	double fc1 = 5;                           // cutoff frequency low
	double fc2 = 60;                          // cutoff freuency high
	double FrequencyBands[2] = { fc1 / Fs ,fc2 / Fs };

	for (int j = 0; j<signal.size(); j++) {

		noisy_imfs[j] = 0;        // fill vector of noisy imfs    

	}


	for (int i = 0; i<num_noised; i++) {
		for (int j = 0; j<signal.size() - 2; j++) {

			noisy_imfs[j] = noisy_imfs[j] + IMFs(i, j);        // fill vector of noisy imfs    

		}
	}


	DenC = ComputeDenCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1]);
	NumC = ComputeNumCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1], DenC);



	noisy_imfs = IIRfilter(DenC, NumC, noisy_imfs); // filter noisy imf with IIR filter


	for (int j = 0; j<noisy_imfs.size(); j++) {//
											   //add sum of noisy imfs to IMFs matrix	
		IMFs(num_noised - 1, j) = noisy_imfs[j];

	}

	for (int i = 0; i<signal.size(); i++) {

		sum_imf[i] = 0;

	}

	for (int i = num_noised - 1; i<IMFs.rows(); i++) {
		for (int j = 0; j<IMFs.cols(); j++) {
			// add all imfs, from num_noised-1 rows, which contains sum of filtred noisy imfs
			imf[j] = IMFs(i, j);
			sum_imf[j] = sum_imf[j] + imf[j];          // add all imfs

		}
	}

	sum_imf = sum_imf;//+res;                           // add imfs with residue

	return sum_imf;

}



int main()
{
	//Enter path of raw signal
	ifstream is(/*Path*/);
	istream_iterator<double> start(is), end;
	vector<double> signalvec(start, end);
	cout << "Read " << signalvec.size() << " numbers" << std::endl;
	VectorXd signal(signalvec.size());
	VectorXd filteredsignal(signalvec.size());
	
	for (int i = 0; i<signalvec.size() - 2; i++) {

		signal[i] = signalvec[i];

	}

	filteredsignal = Denoising(signal);

	for (int i = 0; i<filteredsignal.size(); i++) {

		signalvec[i] = filteredsignal[i];

	}

	//Enter path of output signal 
	ofstream outputFile(/*Enter path*/);

	copy(signalvec.begin(), signalvec.end(), ostream_iterator<double>(outputFile, ", "));




	return 0;
}

