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
    
    market->printActions();

    PnlVect *result = pnl_vect_new();
    
    market->getSpotsFromDate(result, "2014-07-11");
    pnl_vect_print(result);

    market->getSpotsFromDate(result, "2014-07-15");
    pnl_vect_print(result);
    
    market->getSpotsFromDate(result, "2014-07-16");
    
    pnl_vect_print(result);
    pnl_vect_free(&result);

}