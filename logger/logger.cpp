/**
 * I (Scott Durant) worked with Todd Sandburg on this project.
 * We both worked on it at the same time, so the work was split 50/50.
 */

#include "../includes/logger.h"
using namespace std;

Logger::Logger(std::string filename) {
	this->filename = filename;
}

Logger::~Logger() {
}

//open close and clear the log file
void Logger::clearlogfile() {
	lock_guard<mutex> lck(m);
	myFile.open(filename, std::fstream::trunc);

	//close file
	if (myFile.is_open())
		myFile.close();
}

// log given data to the file
void Logger::log(std::string data) {
	lock_guard<mutex> lck(m);
	myFile.open(filename, std::fstream::app);
	if (!myFile.is_open())
		return;

	std::string myline;

	myFile << data;

	//close file
	if (myFile.is_open())
		myFile.close();
}
