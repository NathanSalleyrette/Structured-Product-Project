#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include "fs/ParseYahooCsv.hpp"
#include <vector>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


using namespace std;

class MarketData {
    private:
        map<string, int> stocksIndexes;
        

    public:
        
        map<string, map<string, double>> data;
        vector<string> actions;

        MarketData();
        map<string, map<string, double>> &getData();
        double getSpotFromDateAndAction(string date, string action);
        int getNumOfActions();
        void fillData(ParseYahooCsv *parser);
        void getSpotsFromDate(PnlVect* spots, string date);
        void printActions();

        /**
         * @brief return numbor of stocks considered in the marketdata
         * 
         * @return int 
         */
        // int getNbStocks();

        /**
         * @brief fill a path matrix of all shares on a specific window
         * 
         * @param path the path matrix
         * @param startDate first day of the window 
         * @param nbOfDays size of the window
         */
        void fiilPathMat(PnlMat* path, string startDate, int nbOfDays);

        /**
         * @brief Construct a new fillfrom Path object
         * 
         * @param path the path matrix 
         * @param simulationDates the dates associated
         */
        void fillfromPath(const PnlMat* path, vector<string> simulationDates);
};