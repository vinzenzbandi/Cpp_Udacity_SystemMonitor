#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>
#include "linux_parser.h"
using namespace std;

class Processor {
    public:
        float Utilization();  // DONE: See src/processor.cpp
        float UtilizationDynamic();
        void ParseCpuUtilization();

    // DONE: Declare any necessary private members
    private:
        long prevActiveJiff_ = 0;
        long prevTotalJiff_ = 0;
};

#endif