#include "test_csv.h"
#include "savitzky_golay.h"
#include "simple_file_handler.h"

void test_csv()
{
	SavitzkyGolayFilter SGFilter;
	SimpleFileHandler FileHandler;

	std::vector<float> inputSignal;
	std::vector<float> filteredSignal;
	std::vector<float> filteredSignal_lp;
	std::vector<float> baselineSignal;

	std::string fileName = "100_1";
	std::string path = "signals/csv/";

	unsigned lowPassWindowSize = 15;
	unsigned highPassWindowSize = 85;
	float samplingFreq = 360;
	float cutoffFreq = 5;

	std::cout << "Choose filtering method: " << std::endl << std::endl;
	std::cout << "1 - fixed window high-pass" << std::endl;
	std::cout << "2 - frequency-dependent high-pass" << std::endl;
	std::cout << "3 - two step with both low- and high-pass filtering" << std::endl << std::endl;
	std::cout << "q - quit" << std::endl;

	char actionType;
	std::cin.get(actionType);
	if (actionType != '1' && actionType != '2' && actionType != '3')
		return;

	std::cout << "Loading file... " << std::endl << std::endl;

	FileHandler.readFileToVector(path, fileName + ".csv", inputSignal);

	std::cout << "Starting filtration... " << std::endl << std::endl;

	std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
	switch (actionType)
	{
	case '1':
		SGFilter.filterHighPass(inputSignal, highPassWindowSize, filteredSignal, baselineSignal);
		break;
	case '2':
		SGFilter.filterHighPass(inputSignal, samplingFreq, cutoffFreq, filteredSignal, baselineSignal);
		break;
	case '3':
		SGFilter.filter(inputSignal, lowPassWindowSize, FilterType(lowPass), filteredSignal_lp, baselineSignal);
		SGFilter.filter(filteredSignal_lp, highPassWindowSize, FilterType(highPass), filteredSignal, baselineSignal);
		break;
	default:
		return;
	}
	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	std::cout << "All done!" << std::endl;

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count();
	std::cout << "Filtering took: " << (double)duration / 1000 << " miliseconds" << std::endl;

	FileHandler.writeVectorToFile(path, fileName + "_filtered.csv", filteredSignal);
	if (baselineSignal.size() != 0)
		FileHandler.writeVectorToFile(path, fileName + "_baseline.csv", baselineSignal);

	std::cout << "Everything has been saved, press enter key to exit." << std::endl;
	std::cin.get();
	std::cin.ignore();

	return;
}
