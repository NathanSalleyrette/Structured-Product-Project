#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include "ParseCsv.hpp"
#include <vector>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


using namespace std;

class MarketData {
    private:
        map<string, map<string, double>> data;
        vector<string> actions;

    public:
        MarketData();
        map<string, map<string, double>> &getData();
        double getSpotFromDateAndAction(string date, string action);
        void fillData(ParseCsv *parser);
        void getSpotsFromDate(PnlVect* spots, string date);
        void printActions();

};