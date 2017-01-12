#ifndef SIMPLE_CSV_HANDLER
#define SIMPLE_CSV_HANDLER

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class SimpleCsvHandler {
	public:
		void readFileToVector(std::string path, std::string fileName, std::vector<float> &outSignal);
		void writeVectorToFile(std::string path, std::string fileName, std::vector<float> signal);
};

#endif
