#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include "ParseCsv.hpp"

using namespace std;

class MarketData {
    private:
        map<string, map<string, double>> data;

    public:
        MarketData();
        map<string, map<string, double>> &getData();
        double getSpotFromDateAndAction(string date, string action);
        void fillData(ParseCsv *parser);

};