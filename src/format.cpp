#include <string>
#include "format.h"
using namespace std;

#define SEC_IN_HR 3600
#define SEC_IN_MIN 60


using std::string;

string Format::AddLeadingZero(int number){
/* retrun at least two digits for 
the input number*/
    if (number < 10){
        return "0" + to_string(number);
    }else{
        return to_string(number);
    }
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string hours, min, sec;
    int rest;
    hours = AddLeadingZero( seconds / SEC_IN_HR );
    rest = seconds % SEC_IN_HR;
    min = AddLeadingZero( rest / SEC_IN_MIN );
    sec = AddLeadingZero(rest % SEC_IN_MIN);
    return hours + ":" + min + ":" + sec;
}