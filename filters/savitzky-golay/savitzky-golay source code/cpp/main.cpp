#include <iostream>
#include <string>
#include <chrono>

#include "savitzky_golay.h"
#include "simple_dat_handler.h"
#include "filter_type.h"
#include "test_batch.h"
#include "test_csv.h"

int main()
{
	std::vector<std::string> fileNames;

	for (int i = 100; i < 110; ++i)
	{
		fileNames.push_back(std::to_string(i));
	}
	std::string inputPath = "signals/dat_input/";
	std::string outputPath = "signals/dat_output/";

	test_batch(fileNames, inputPath, outputPath);

	//test_csv();

	return 0;
}
