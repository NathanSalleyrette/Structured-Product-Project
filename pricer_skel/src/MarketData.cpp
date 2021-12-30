#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <filesystem>
#include <sstream>
#include "MarketData.hpp"
#include "ParseCsv.hpp"

using std::filesystem::directory_iterator;


MarketData::MarketData() {}

map<string, map<string, double>> & MarketData::getData() {
    return data;
}

double MarketData::getSpotFromDateAndAction(string date, string action) {
    return data[date][action];
}

void MarketData::fillData(ParseCsv *parser) {
    string pathFiles = "../data/DATA";
    string action;
    int ind = 0;
    for (const auto & entry : directory_iterator(pathFiles)) {
        action = entry.path();
        action.erase(action.end()-4, action.end());
        action.erase(action.begin(), action.begin()+13);
        parser->setCheminData(entry.path());
        parser->fillDictFromFile(data, action);
    }
} 