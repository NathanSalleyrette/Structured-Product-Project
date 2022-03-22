#include <iostream>
#include <string>
#include "fs/ParseYahooCsv.hpp"
#include "financialProducts/Performance.hpp"
#include "utils/MarketData.hpp"
#include <map>
#include <vector>
#include "spdlog/log.hpp"
#include "utils/Date.hpp"

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
     , "2021-01-11", "2021-07-11"
     , "2022-01-11", "2022-07-11"};


    vector<string> observeDates = {};

    Performance *perf = new Performance(observeDates, market, observeDates);
    PnlMat* path = pnl_mat_new();
    

    perf->niveauInitial();



    vector<string> datesFrom2014To20210107 = Date::getListOfDates("2014-07-16", "2022-01-06");

    // Initialisation
    int counter = 0;
    observeDates.push_back(datesFrom2014To20210107[0]);

    for (int i = 1; i < datesFrom2014To20210107.size(); i++) {
        if (datesFrom2014To20210107[i] > dates[counter]) {
            observeDates.push_back(datesFrom2014To20210107[i]);
            counter += 1;
        }else {
            observeDates.back() = datesFrom2014To20210107[i];
        }

        perf->setObservationDates(observeDates);

        market->getPathFromDates(path, observeDates);
        pnl_mat_set_row(path, perf->getNivInitAct(), 0);

        double result = perf->calculPerfMoyenneFinale(path);
        string dialogue = "Perf moy finale du panier au " + datesFrom2014To20210107[i] + " : ";
        cout << dialogue << result << "%" << endl;
    }
    
    pnl_mat_free(&path);

    delete perf;
    delete market;
    delete parser;

}