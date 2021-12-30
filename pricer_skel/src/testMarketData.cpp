#include "MarketData.hpp"
#include "ParseCsv.hpp"
#include <map>

int main() {
    ParseCsv *parser = new ParseCsv(7);
    parser->setCheminData("../data/DATA/FLS.csv");

    MarketData *market = new MarketData();
    //parser->fillDictFromFile(market->getData());
    market->fillData(parser);

    cout << market->getSpotFromDateAndAction("2021-11-11", "FLS") << endl;
}