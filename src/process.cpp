#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id){
    id_ = id;
    user_ = LinuxParser::User(id);
    command_ = LinuxParser::Command(id); 
    ram_ = LinuxParser::Ram(id); 
    upTime_ = LinuxParser::UpTime(id);//TODO
    cpuUtilization_ = CalcUtilization();//TODO
}

float Process::CalcUtilization(){
    long totalJiffies = LinuxParser::Jiffies(id_);
    long processJiffies = LinuxParser::ActiveJiffies(id_);
    return (float)processJiffies / (float)totalJiffies; 
}

// TODO: Return this process's ID
int Process::Pid() const { 
    return id_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return cpuUtilization_;
}

// TODO: Return the command that generated this process
string Process::Command() const { 
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() const { 
    return ram_;
 }

// TODO: Return the user (name) that generated this process
string Process::User() const { 
    return user_;
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { 
    return upTime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process const& a) const { 
    return CpuUtilization() > a.CpuUtilization();
}