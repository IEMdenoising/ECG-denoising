#ifndef SIMPLE_FILE_HANDLER
#define SIMPLE_FILE_HANDLER

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class SimpleFileHandler {
public:
	void readFileToVector(std::string path, std::string fileName, std::vector<float> &outSignal);
	void writeVectorToFile(std::string path, std::string fileName, std::vector<float> signal);
};

#endif
