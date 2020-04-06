#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
// Return the aggregate CPU utilization
float Processor::Utilization() { 
	std::string line;
	std::string key;
	int pre_user, pre_nice, pre_system, pre_idle, pre_iowait, pre_irq, pre_softirq, pre_steal, pre_guest, pre_guest_nice;
	int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

	int Pre_Idle, Idle, Pre_NonIdle, NonIdle, Pre_Total, Total, totald, idled;
	float CPU_Percentage = 0.0;
	
	std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> key >> pre_user >> pre_nice >> pre_system >> 
		pre_idle >> pre_iowait >> pre_irq >> pre_softirq >> 
		pre_steal >> pre_guest >> pre_guest_nice;	
	}
	
	usleep(50000);
	// read the file twice
	std::ifstream stream2(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
	if (stream2.is_open()) {
		std::getline(stream2, line);
		std::istringstream linestream(line);
		linestream >> key >> user >> nice >> system >> idle >> iowait
		>> irq >> softirq >> steal >> guest >> guest_nice;	
	}
	
	Pre_Idle = pre_idle + pre_iowait;
	Idle = idle + iowait;

	Pre_NonIdle = pre_user + pre_nice + pre_system + pre_irq + pre_softirq + pre_steal;
	NonIdle = user + nice + system + irq + softirq + steal;

	Pre_Total = Pre_Idle + Pre_NonIdle;
	Total = Idle + NonIdle;

	totald = Total - Pre_Total;
	idled = Idle - Pre_Idle;

	CPU_Percentage = (totald - idled)/float(totald);

	return CPU_Percentage; 
}