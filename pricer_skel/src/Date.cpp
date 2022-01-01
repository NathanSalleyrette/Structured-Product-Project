#include "Date.hpp"
#include <stdio.h>
#include <stdlib.h>

using std::to_string;


string formateNumber(int n) {
    if (n<10) {
        return "0" + to_string(n);
    } else {
        return to_string(n);
    }
}

string Date::nextDate(string date) {
    time_t t;
    date += "-12:00:00";
    struct tm stm;
    struct tm *next;
    strptime(date.c_str(), "%Y-%m-%d-%H:%M:%S", &stm);
    stm.tm_isdst = -1;
    t = mktime(&stm);
    t += 86400;
    next = localtime(&t);
    int year = next->tm_year + 1900;
    int month =  next->tm_mon + 1;
    int day =  next->tm_mday;
    string nextDate = "" + to_string(year) + "-" + formateNumber(month) + "-" + formateNumber(day);
    return nextDate;
}
