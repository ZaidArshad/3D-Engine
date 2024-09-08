#include <iostream>
#include <sstream>

#include "logger.h"

Logger Logger::instance;

Logger::Logger()
{
	instance.outFile = std::ofstream("log.txt");
}

void Logger::log(const std::string &msg)
{
	std::cout << msg << std::endl;
	instance.outFile << msg << std::endl;
}
