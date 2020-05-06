#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long activeJiff = LinuxParser::ActiveJiffies();
    long idleJiff = LinuxParser::IdleJiffies();
    long totalJiff = activeJiff + idleJiff;
    return ((float)activeJiff / (float)totalJiff); 
}

float Processor::UtilizationDynamic() { 
/* calulates the usage since it was invoced the last time */
    long activeJiff = LinuxParser::ActiveJiffies();
    long idleJiff = LinuxParser::IdleJiffies();
    long totalJiff = activeJiff + idleJiff;
    long dActiveJiff = activeJiff - prevActiveJiff_;
    long dTotalJiff = totalJiff - prevTotalJiff_;

    prevActiveJiff_ = activeJiff;
    prevTotalJiff_ = totalJiff;
    return ((float)dActiveJiff / (float)dTotalJiff); 
}