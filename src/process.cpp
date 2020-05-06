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

Process::Process(int id): id_(id){
    user_ = LinuxParser::User(id_);
    command_ = LinuxParser::Command(id_); 
    ram_ = LinuxParser::Ram(id_); 
    upTime_ = LinuxParser::UpTime(id_);
    cpuUtilization_ = CalcUtilization();
}

float Process::CalcUtilization(){
    float procSeconds;
    long procJiffies = LinuxParser::ActiveJiffies(id_);
    long startJiffies = LinuxParser::StartTime(id_);
    long systemTime = LinuxParser::UpTime();

    // diff since previous execution
    long dProcJiffies = procJiffies - prevProcJiffies_;
    long dStartJiffies = startJiffies - prevStartJiffies_;
    long dSystemTime = systemTime - prevSystemTime_;

    procSeconds = dSystemTime - (dStartJiffies / sysconf(_SC_CLK_TCK));

    prevProcJiffies_ = procJiffies;
    prevStartJiffies_ = startJiffies;
    prevSystemTime_ = systemTime;
    
    return (dProcJiffies / sysconf(_SC_CLK_TCK)) / procSeconds;
}

// DONE: Return this process's ID
int Process::Pid() const { 
    return id_;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return cpuUtilization_;
}

// DONE: Return the command that generated this process
string Process::Command() const { 
    return command_;
}

// DONE: Return this process's memory utilization
string Process::Ram() const { 
    return ram_;
 }

// DONE: Return the user (name) that generated this process
string Process::User() const { 
    return user_;
 }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { 
    return upTime_;
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process const& a) const { 
    return CpuUtilization() > a.CpuUtilization();
}