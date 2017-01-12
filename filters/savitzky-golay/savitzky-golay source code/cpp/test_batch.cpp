#include "test_batch.h"
#include "savitzky_golay.h"
#include "simple_dat_handler.h"

void test_batch(std::vector<std::string> fileNames, std::string inputPath, std::string outputPath)
{
	SavitzkyGolayFilter SGFilter;
	SimpleDatHandler CsvHandler;

	unsigned lowPassWindowSize = 11;
	unsigned highPassWindowSize = 85;
	float samplingFreq = 360;
	float cutoffFreq = 5;
	
	std::vector<float> executionTimes;

	std::cout << "Choose filtering method: " << std::endl << std::endl;
	std::cout << "1 - fixed window high-pass" << std::endl;
	std::cout << "2 - frequency-dependent high-pass" << std::endl;
	std::cout << "3 - two step with both low- and high-pass filtering" << std::endl << std::endl;
	std::cout << "q - quit" << std::endl;

	char actionType;
	std::cin.get(actionType);
	if (actionType != '1' && actionType != '2' && actionType != '3')
		return;

	for (std::vector<std::string>::iterator it = fileNames.begin(); it < fileNames.end(); ++it)
	{
		std::vector<float> inputSignal;
		std::vector<float> filteredSignal;
		std::vector<float> filteredSignal_lp;
		std::vector<float> baselineSignal;

		std::cout << "Loading file " << *it << "... " << std::endl << std::endl;

		CsvHandler.readFileToVector(inputPath, *it + ".dat", inputSignal);

		std::cout << "Starting filtration... " << std::endl << std::endl;

		std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
		switch (actionType)
		{
			case '1':
				SGFilter.filterHighPass(inputSignal, highPassWindowSize, filteredSignal);
				break;
			case '2':
				SGFilter.filterHighPass(inputSignal, samplingFreq, cutoffFreq, filteredSignal);
				break;
			case '3':
				SGFilter.filter(inputSignal, lowPassWindowSize, FilterType(lowPass), filteredSignal_lp);
				SGFilter.filter(filteredSignal_lp, highPassWindowSize, FilterType(highPass), filteredSignal);
				break;
			default:
				return;
		}
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count();
		std::cout << "Filtering took: " << (float)duration / 1000 << " miliseconds" << std::endl;

		executionTimes.push_back(duration);

		std::cout << "Saving..." << std::endl;

		CsvHandler.writeVectorToFile(outputPath, *it + "_filtered.dat", filteredSignal);
	}

	CsvHandler.writeVectorToFile(outputPath, "execution_times.dat", executionTimes);

	std::cout << "All done!" << std::endl;
	std::cout << "Everything has been saved, press enter key to exit." << std::endl;

	std::cin.get();
	std::cin.ignore();

	return;
}
