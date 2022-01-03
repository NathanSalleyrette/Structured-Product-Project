#include <iostream>
#include <string>
#include "ParseYahooCsv.hpp"
#include "Performance.hpp"
#include "MarketData.hpp"
#include <map>
#include <vector>

using namespace std;


int main() {
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

    Performance *perf = new Performance(observeDates, market);


    perf->niveauInitial();

    for (int i = 0; i < dates.size(); i++) {
        observeDates.push_back(dates[i]);
        perf->setObservationDates(observeDates);
        //perf->printObservationDates();
        //cout << endl;
        double result = perf->calculPerfMoyenneFinale();
        string dialogue = "Perf moy finale du panier au " + dates[i] + " : ";
        cout << dialogue << result << "%" << endl;
    }
    
    delete perf;

}