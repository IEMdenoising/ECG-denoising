#ifndef SIMPLE_DAT_HANDLER
#define SIMPLE_DAT_HANDLER

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class SimpleDatHandler {
public:
	void readFileToVector(std::string path, std::string fileName, std::vector<float> &outSignal);
	void writeVectorToFile(std::string path, std::string fileName, std::vector<float> signal);
};

#endif
