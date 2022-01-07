#include <string>
#include "../../headers/MarketData.hpp"
#include "../../headers/fs/ParseYahooCsv.hpp"
#include "../../headers/utils/Utils.hpp"
#include <map>

using namespace std;

int main(int argc, char **argv)
{

    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser);

    PnlMat* path = pnl_mat_create(1,1);
    market->fiilPathMat(path, "2018-11-11", 10);


    PnlVect* volatilities = pnl_vect_create(market->getNbStocks());
    Utils::volsOnMat(volatilities, path);

    pnl_vect_free(&volatilities);
    pnl_mat_free(&path); 
}