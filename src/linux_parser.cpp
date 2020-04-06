#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
	while (std::getline(filestream, line)) {
	  std::replace(line.begin(), line.end(), ' ', '_');
	  std::replace(line.begin(), line.end(), '=', ' ');
	  std::replace(line.begin(), line.end(), '"', ' ');
	  std::istringstream linestream(line);
	  while (linestream >> key >> value) {
		if (key == "PRETTY_NAME") {
		  std::replace(value.begin(), value.end(), '_', ' ');
		  return value;
		}
	  }
	}
  }
  return value;
}

// Read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
	std::getline(stream, line);
	std::istringstream linestream(line);
	linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
	// Is this a directory?
	if (file->d_type == DT_DIR) {
	  // Is every character of the name a digit?
	  string filename(file->d_name);
	  if (std::all_of(filename.begin(), filename.end(), isdigit)) {
		int pid = stoi(filename);
		pids.push_back(pid);
	  }
	}
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
	std::string line;
	std::string key;
	float value;
	std::ifstream stream(kProcDirectory + kMeminfoFilename);
	int memTotal = 0;
	int memFree = 0;
	float memUsed = 0.0;
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "MemTotal:") {
					memTotal = value;
				}
				if (key == "MemFree:") {
					memFree = value;
				}
			}
		}
	}
	memUsed = (memTotal - memFree) / float(memTotal);

	return memUsed; 
}

//  Read and return the system uptime
long LinuxParser::UpTime() { 
	std::string line;
	long value1 = 0;
	long value2 = 0;

	std::ifstream stream(kProcDirectory + kUptimeFilename);
	
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> value1 >> value2;	
	}
	return value1; 
}

//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

//  Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
	string line;
	string key;
	int value;
	std::ifstream stream(kProcDirectory + kStatFilename);
	
	int totalProcesses = 0;

	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "processes") {
					totalProcesses = value;
				}
			}
		}
	}
	return totalProcesses; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
	string line;
	string key;
	int value;
	std::ifstream stream(kProcDirectory + kStatFilename);
	
	int runningProcs = 0;

	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "procs_running") {
					runningProcs = value;
				}
			}
		}
	}
	return runningProcs; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
	string line;
	string key;
	string value;
	string s_pid = std::to_string(pid);
	std::ifstream stream(kProcDirectory + '/' + s_pid + kStatusFilename);
	
	string UID = "NONE";

	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "Uid:") {
					UID = value;
				}
				
			}
		}
	}
	return UID; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
	string line;
	string key1, key2;
	string value;
	std::ifstream filestream(kPasswordPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			while (linestream >> key1 >> key2 >> value) {
				if (value == Uid(pid)) {
					return key1;
				}
			}
		}
	}
	return "NONE"; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }