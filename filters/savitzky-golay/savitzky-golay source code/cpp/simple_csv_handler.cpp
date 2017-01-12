#include "simple_csv_handler.h"

void SimpleCsvHandler::readFileToVector(std::string path, std::string fileName, std::vector<float> &outSignal)
{
	std::ifstream inf(path.append(fileName));

	if (!inf)
	{
		std::cerr << "File not opened." << std::endl;
		return;
	}

	std::string strInput;
	float input;
	while (inf)
	{
		std::getline(inf, strInput);
		try {
			input = std::stof(strInput);
		}
		catch (const std::exception& e) {
			break;
		}
		outSignal.push_back(input);
	}
}

void SimpleCsvHandler::writeVectorToFile(std::string path, std::string fileName, std::vector<float> signal)
{
	std::ofstream file;
	file.open(path.append(fileName));

	if (!file)
	{
		std::cerr << "File not opened." << std::endl;
		return;
	}

	for (std::vector<float>::iterator it = signal.begin(); it < signal.end(); ++it) {
		file << *it << std::endl;
	}

	file.close();
}
