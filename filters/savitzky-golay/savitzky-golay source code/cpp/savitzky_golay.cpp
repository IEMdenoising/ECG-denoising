#include "savitzky_golay.h"

void SavitzkyGolayFilter::filter(std::vector<float> input, unsigned int windowSize, FilterType filterType, std::vector<float> &outSignal, std::vector<float> &outBaseline)
{
	switch(filterType)
	{
		case lowPass:
			filterHighPass(input, windowSize, outBaseline, outSignal);	// mind the reverted out args' order!
			break;
		case highPass:
		default:
			filterHighPass(input, windowSize, outSignal, outBaseline);
			break;
	}
}

void SavitzkyGolayFilter::filterHighPass(std::vector<float> input, float samplingFreq, float cutoffFreq, std::vector<float> &outSignal, std::vector<float> &outBaseline)
{
	filterHighPass(input, getWindowSize(samplingFreq, cutoffFreq), outSignal, outBaseline);
}

void SavitzkyGolayFilter::filterHighPass(std::vector<float> input, unsigned int windowSize, std::vector<float> &outSignal, std::vector<float> &outBaseline)
{
	smoothingArray = getSmoothingArray(windowSize);

	outSignal.resize(input.size());
	outBaseline.resize(input.size());

	std::vector<float> inputExtended = extendInput(input);

	outSignal.resize(input.size());
	outBaseline.resize(input.size());
	std::fill(outBaseline.begin(), outBaseline.end(), 0.0);

	unsigned smoothingArraySize = smoothingArray.size();
	for (unsigned i = 0; i<input.size(); ++i) {
		for (unsigned j = 0; j < smoothingArraySize; ++j) {
			outBaseline[i] += smoothingArray[j]*inputExtended[(i + (smoothingArraySize - 1)) - j];
		}
		outSignal[i] = input[i] - outBaseline[i];
	}
}

std::vector<float> SavitzkyGolayFilter::extendInput(std::vector<float> input)
{
	unsigned m = smoothingArray.size() / 2;

	std::vector<float> inputExtended;
	inputExtended.resize(m);

	float frontValue = *input.begin();
	for (std::vector<float>::iterator it = inputExtended.begin(); it < inputExtended.begin() + m; ++it) {
		*it = frontValue;
	}

	inputExtended.insert(inputExtended.end(), input.begin(), input.end());
	inputExtended.resize(input.size() + 2 * m);

	float backValue = *(input.end()-1);
	for (std::vector<float>::iterator it = inputExtended.end() - m; it < inputExtended.end(); ++it) {
		*it = backValue;
	}

	return inputExtended;
}

unsigned SavitzkyGolayFilter::getWindowSize(float samplingFreq, float cutoffFreq)
{
	return oddifyWindowSize((unsigned)floor(1.25*samplingFreq / cutoffFreq + 1.4375));
}

unsigned SavitzkyGolayFilter::oddifyWindowSize(unsigned windowSize)
{
	return windowSize % 2 ? windowSize : windowSize + 1;
}

std::vector<float> SavitzkyGolayFilter::getSmoothingArray(unsigned windowSize)
{
	windowSize = oddifyWindowSize(windowSize);

	std::vector<float> smoothingArray;
	smoothingArray.resize(windowSize);

	int m = windowSize / 2;

	for (int n = -m; n < m + 1; n++) {
		double msquared = (double)m*m;
		smoothingArray[n + m] = (9.0*msquared + 9.0*m - 3.0 - 15.0*(double)(n*n)) / ((2.0*(double)m + 3.0)*(4.0*msquared - 1.0));
	}

	return smoothingArray;
}
