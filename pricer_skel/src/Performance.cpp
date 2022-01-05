#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include "Performance.hpp"


Performance::Performance(vector<string> observationDates, MarketData *md) {
    parser = new ParseYahooCsv();
    this->observationDates = observationDates;
    nivInitAct = pnl_vect_create_from_zero(md->getNumOfActions());
    this->md = md;
}

Performance::~Performance() {
    pnl_vect_free(&nivInitAct);
    delete parser;
}


double Performance::payoff() {
    return 0.;
}

void Performance::niveauInitial() {

    PnlVect * add = pnl_vect_create(md->getNumOfActions());

    md->getSpotsFromDate(nivInitAct, "2014-07-11");
    md->getSpotsFromDate(add, "2014-07-15");
    
    pnl_vect_plus_vect(nivInitAct, add);
    
    md->getSpotsFromDate(add, "2014-07-16");
    pnl_vect_plus_vect(nivInitAct, add);

    pnl_vect_div_scalar(nivInitAct, 3.);
    pnl_vect_free(&add);
}


double roundFourDecimal(double d) {
    return round(d * 10000.0) / 10000.0;
}


double Performance::calculPerfMoyenneFinale() {

    double perfMoyenne = 0.;
    vector<string>::iterator it;
    for (it = observationDates.begin(); it != observationDates.end(); it++) {
            perfMoyenne += calculPerfDate(*it);
    }
    return 100*roundFourDecimal(perfMoyenne / observationDates.size());
}

// Calcul la performance des 30 actions sur un indice donnée
// qui correspond à l'indice de la date souhaitée dans le vecteur observationDates
double Performance::calculPerfDate(string date) {

    double perfDate = 0.;
    PnlVect *actions = pnl_vect_create(md->getNumOfActions());
    md->getSpotsFromDate(actions, date);
    for (int i = 0; i < actions->size; i++) {
            double nivInit = GET(nivInitAct, i);
            double perfAction = (GET(actions, i) - nivInit) / nivInit;
            perfDate += roundFourDecimal(perfAction);
    }
    return std::max(perfDate / actions->size, 0.);
}

void Performance::printObservationDates() {
    for (int i = 0; i < observationDates.size(); i++) {
        cout << observationDates[i] << " ";
    }
}

void Performance::setObservationDates(vector<string> od) {
    observationDates = od;
}


