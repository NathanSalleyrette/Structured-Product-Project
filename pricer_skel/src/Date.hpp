#pragma once

#include <ctime>
#include <iostream>
#include <sstream>

using std::string;

class Date {
    public:
        Date();
        static string nextDate(string date);
};