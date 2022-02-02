#include <iostream>
#include <string>
#include "fs/ParseYahooCsv.hpp"
#include "financialProducts/Performance.hpp"
#include "utils/MarketData.hpp"
#include <map>
#include <vector>
#include "spdlog/log.hpp"

using namespace std;


int main() {
    log::init();
    ParseYahooCsv *parser = new ParseYahooCsv();
    
    MarketData *market = new MarketData();
    
    market->fillData(parser);

   
    vector<string> dates = 
     { "2015-01-12", "2015-07-13"
     , "2016-01-11", "2016-07-11"
     , "2017-01-11", "2017-07-11"
     , "2018-01-11", "2018-07-11"
     , "2019-01-11", "2019-07-11"
     , "2020-01-11", "2020-07-11"
     , "2021-01-11", "2021-07-11"};

    vector<string> observeDates = {};

    Performance *perf = new Performance(observeDates, market, observeDates);
    PnlMat* path = pnl_mat_new();
    PnlMat* pathFull = pnl_mat_new();
    

    perf->niveauInitial();

    market->getPathFromDates(pathFull, dates);
    pnl_mat_set_row(pathFull, perf->getNivInitAct(), 0);

    for (int i = 0; i < dates.size(); i++) {
        observeDates.push_back(dates[i]);
        perf->setObservationDates(observeDates);

        market->getPathFromDates(path, observeDates);
        pnl_mat_set_row(path, perf->getNivInitAct(), 0);

        double result = perf->calculPerfMoyenneFinale(path);
        string dialogue = "Perf moy finale du panier au " + dates[i] + " : ";
        cout << dialogue << result << "%" << endl;
    }
    
    pnl_mat_free(&path);
    pnl_mat_free(&pathFull);
    delete perf;
    delete market;
    delete parser;

}