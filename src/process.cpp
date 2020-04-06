#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"
#include <iostream>
using std::string;
using std::to_string;
using std::vector;

Process::Process(int id): id(id){}

// Return this process's ID
int Process::Pid() { return id; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{ 
	string value;
	int count = 0;
	float uptime = 0, utime = 0, stime = 0, cutime = 0, cstime = 0, starttime = 0;
	float total_time = 0.0;
	float seconds;
	int Hertz = sysconf(_SC_CLK_TCK);
	float cpu_usage = 0.0;
	string line;
	string s_pid = std::to_string(id);

	uptime = LinuxParser::UpTime();
	std::ifstream stream(LinuxParser::kProcDirectory + '/' + s_pid + LinuxParser::kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		while (linestream >> value) {
			count++; 
			if (count == 14) {
				utime = std::stof(value);
			}
			else if (count == 15) {
				stime = std::stof(value);
			}
			else if (count == 16) {
				cutime = std::stof(value);
			}
			else if (count == 17) {
				cstime = std::stof(value);
			}
			else if (count == 22) {
				starttime = std::stof(value);
			}
		}
	}
	total_time = utime + stime + cstime + cutime;
	seconds = uptime - starttime / Hertz;	
	cpu_usage =  total_time / Hertz / seconds;

	return cpu_usage;
	
}

// Return the command that generated this process
string Process::Command() { 
	string line;
	string s_pid = std::to_string(id);
	std::ifstream stream(LinuxParser::kProcDirectory + '/' + s_pid + LinuxParser::kCmdlineFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
	}
	return line; 
}

// Return this process's memory utilization
string Process::Ram() { 
	string key;
	int value;
	string ram;
	string line;
	string s_pid = std::to_string(id);

	std::ifstream stream(LinuxParser::kProcDirectory + '/' + s_pid + LinuxParser::kStatusFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "VmSize:") {
					ram = std::to_string(value/1000);
				}
				
			}
		}
	}
	return ram;
}

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(id); }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
	string value;
	long int starttime = 0;
	int Hertz = sysconf(_SC_CLK_TCK);
	int count = 0;
	string line;
	string s_pid = std::to_string(id);
	std::ifstream stream(LinuxParser::kProcDirectory + '/' + s_pid + LinuxParser::kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		while (linestream >> value) {
		count++;
			if (count == 22) {
				starttime = std::stof(value);
			}
		}
	}
	return starttime / Hertz; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
	return CpuUtilization() < a.CpuUtilization();
}