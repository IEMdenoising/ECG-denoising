#ifndef SAVITZKY_GOLAY_FILTER
#define SAVITZKY_GOLAY_FILTER

#include<vector>
#include "filter_type.h"

class SavitzkyGolayFilter {
	public:
		void filterHighPass(std::vector<float> input, float samplingFreq, float cutoffFreq, std::vector<float> &outSignal, std::vector<float> &outBaseline = std::vector<float>());
		void filterHighPass(std::vector<float> input, unsigned int windowSize, std::vector<float> &outSignal, std::vector<float> &outBaseline = std::vector<float>());
		void filter(std::vector<float> input, unsigned int windowSize, FilterType filterType, std::vector<float> &outSignal, std::vector<float> &outBaseline = std::vector<float>());
	private:
		std::vector<float> extendInput(std::vector<float> input);
		unsigned getWindowSize(float samplingFreq, float cutoffFreq);
		unsigned oddifyWindowSize(unsigned windowSize);
		std::vector<float> getSmoothingArray(unsigned windowSize);
		std::vector<float> smoothingArray;
};

#endif
