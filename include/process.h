#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id);
  int Pid() const;                               // DONE: See src/process.cpp
  std::string User() const;                      // DONE: See src/process.cpp
  std::string Command() const;                   // DONE: See src/process.cpp
  float CpuUtilization() const;                  // DONE: See src/process.cpp
  std::string Ram() const;                       // DONE: See src/process.cpp
  long int UpTime() const;                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  bool operator>(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
    float CalcUtilization();
    int id_;
    std::string user_;
    std::string command_;
    float cpuUtilization_;
    std::string ram_;
    long int upTime_;
    long prevProcJiffies_ = 0;
    long prevStartJiffies_ = 0;
    long prevSystemTime_ = 0;
};

#endif