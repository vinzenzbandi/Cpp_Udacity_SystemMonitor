#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
/* calculated as the difference between MemTotal and MemFree*/
  string line, key, value;
  int memFree = 0;
  int memTotal = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stoi(value);
      }else if (key == "MemFree:"){
        memFree = std::stoi(value);
      }
    }
  }
  float utilization = (((float)memTotal - (float)memFree) / (float)memTotal);
  return utilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, value;
  long uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '.', ' ');
    std::istringstream linestream(line);
    linestream >> value;
    uptime = std::stoi(value);
  }
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::StartTime(int pid) {
  vector<string> statValues = CpuUtilization(pid);
  if (statValues.size() > 22){
    long startJiffies = stol(statValues[ProcStates::kStartTime]);
    return startJiffies;
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long activeJiffies;
  vector<string> statValues = CpuUtilization(pid);
  if (statValues.size() > 22){
    long utime = stol(statValues[ProcStates::kUTime_]);
    long stime = stol(statValues[ProcStates::kSTime_]);
    long cutime = stol(statValues[ProcStates::kCUTime_]);
    long cstime = stol(statValues[ProcStates::kCSTime_]);
    activeJiffies = utime + stime + cutime + cstime;
    return activeJiffies; 
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuUsage = LinuxParser::CpuUtilization();
  long user = stol(cpuUsage[CPUStates::kUser_]);
  long nice = stol(cpuUsage[CPUStates::kNice_]);
  long system = stol(cpuUsage[CPUStates::kSystem_]);
  long irq = stol(cpuUsage[CPUStates::kIRQ_]);
  long softirq = stol(cpuUsage[CPUStates::kSoftIRQ_]);
  long steal = stol(cpuUsage[CPUStates::kSteal_]);
  return user + nice + system + irq + softirq + steal;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuUsage = LinuxParser::CpuUtilization();
  long idle = stol(cpuUsage[CPUStates::kIdle_]);
  long iowait = stol(cpuUsage[CPUStates::kIOwait_]);
  return idle + iowait;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, number;
  vector<string> utilization{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> number){
      utilization.push_back(number);
    }
  }
  return utilization;
}


vector<string> LinuxParser::CpuUtilization(int pid) { 
  string line, element;
  vector<string> utilization{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> element){
      utilization.push_back(element);
    }
  }
  return utilization;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  int processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = std::stoi(value);
        return processes;
      }
    }
  }
  return processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int running = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        running = std::stoi(value);
        return running;
      }
    }
  }
  return running;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  int ramKb;
  string ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:"){
        ramKb = stoi(value);
        ram = to_string(ramKb / 1000);
        return ram;
      }
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  string uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, filler, value;
  string user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> filler >> value;
      if (value  ==  uid){
        return user;
      }
    }
  }
  return user;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long sysUpTime = UpTime();
  vector<string> statValues = CpuUtilization(pid);
  if (statValues.size() > 22){
    long pidStartTime = stol(statValues[ProcStates::kStartTime]) / sysconf(_SC_CLK_TCK);
    return sysUpTime - pidStartTime;
  }
  return 0;
}