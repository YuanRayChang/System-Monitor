#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
	int hours = seconds/3600;
	int minutes = (seconds - hours*3600)/60;
	int sec = seconds - hours*3600 - minutes*60;

	string HH = std::to_string(hours);
	string MM = std::to_string(minutes);
	string SS = std::to_string(sec);

	return HH + ":" + MM + ":" + SS;
}