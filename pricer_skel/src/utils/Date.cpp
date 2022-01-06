#include "../../headers/utils/Date.hpp"
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

string nextOrPreviousDate(string date, double direction) {
    
    time_t t;
    date += "-12:00:00";
    struct tm stm;
    struct tm *next;
    strptime(date.c_str(), "%Y-%m-%d-%H:%M:%S", &stm);
    stm.tm_isdst = -1;
    if (stm.tm_wday == 1 && direction < 0) direction *= 3;
    else if (stm.tm_wday == 5 && direction > 0) direction *= 3;
        // Si on est samedi
    else if (stm.tm_wday == 6) {
            if (direction > 0) direction *= 2;
        }
    // Si on est dimanche
    else if (stm.tm_wday == 0) {
        if (direction < 0) direction *= 2;
    }

    t = mktime(&stm);
    t += direction * 86400;
    next = localtime(&t);
    int year = next->tm_year + 1900;
    int month =  next->tm_mon + 1;
    int day =  next->tm_mday;
    string nextDate = "" + to_string(year) + "-" + formateNumber(month) + "-" + formateNumber(day);
    return nextDate;
}

// A faire, enlever les samedis et les dimanches
string Date::nextDate(string date) {
    return nextOrPreviousDate(date, 1.);
}

string Date::previousDate(string date) {
    return nextOrPreviousDate(date, -1.);
}



bool Date::isSaturdayOrSunday(string date) {
    date += "-12:00:00";
    struct tm stm;
    strptime(date.c_str(), "%Y-%m-%d-%H:%M:%S", &stm);
    stm.tm_isdst = -1;
    return stm.tm_wday == 0 || stm.tm_wday == 6;
}

vector<string> Date::getListOfDates(string begin_date, string end_date) {
    // Verifier que les deux dates ne sont pas des samedis ou dimanches
    vector<string> list_dates = {begin_date};
    string next = begin_date;
    while (next != end_date) {
        next = nextDate(next);
        list_dates.push_back(next);
    }
    return list_dates;

}

