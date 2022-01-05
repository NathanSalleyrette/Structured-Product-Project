#pragma once

#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

using std::string;

class Date {
    public:
        Date();
        static string nextDate(string date);
};