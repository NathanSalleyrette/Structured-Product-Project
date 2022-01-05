#include "../../headers/MarketData.hpp"
#include "../../headers/fs/ParseYahooCsv.hpp"
#include <map>

int main() {
    ParseYahooCsv *parser = new ParseYahooCsv();
    parser->setCheminData("../data/DATA/FLS.csv");

    MarketData *market = new MarketData();
    
    market->fillData(parser);

    cout << market->getSpotFromDateAndAction("2021-11-11", "FLS") << endl;
    
    market->printActions();

    PnlVect *result = pnl_vect_create(market->getNumOfActions());
    
    market->getSpotsFromDate(result, "2014-07-11");
    pnl_vect_print(result);

    market->getSpotsFromDate(result, "2014-07-15");
    pnl_vect_print(result);
    
    market->getSpotsFromDate(result, "2014-07-16");
    
    pnl_vect_print(result);
    pnl_vect_free(&result);

}