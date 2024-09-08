#pragma once

#include <string>
#include <fstream>

// Couldn't figure out how to cleanly singleton this
// Refered to this for help https://stackoverflow.com/a/64202104

class Logger
{
public:
	static void log(const std::string &msg);

private:
	Logger();

	std::ofstream outFile;

	// Singleton instance
	static Logger instance;
};
