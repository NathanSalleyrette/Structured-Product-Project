#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include <iostream>
#include <filesystem>
#include "Performance.hpp"

using std::filesystem::directory_iterator;

Performance::Performance(PnlVect *observationDates, int nbAction) {
    parser = new ParseCsv(7);
    this->observationDates = observationDates;
    path = pnl_mat_create_from_zero(observationDates->size, nbAction);
    nivInitAct = pnl_vect_create_from_zero(nbAction);
}

Performance::~Performance() {
    pnl_vect_free(&nivInitAct);
    pnl_mat_free(&path);
    delete parser;
}


double Performance::payoff() {
    return 0.;
}

void Performance::niveauInitial() {
    string pathFiles = "../data/DATA";
    int ind = 0;
    for (const auto & entry : directory_iterator(pathFiles)) {
        parser->setCheminData(entry.path());
        parser->getData();
        LET(nivInitAct, ind) = (parser->findClotureFromDate(20140711) + 
            parser->findClotureFromDate(20140715) + parser->findClotureFromDate(20140716))/3;
        ind++;
    }
}

void Performance::setPath() {
    string pathFiles = "../data/DATA";
    int j = 0;
    for (const auto & entry : directory_iterator(pathFiles)) {
        parser->setCheminData(entry.path());
        parser->getData();
        for (int i = 0; i < observationDates->size; i++) {
            double date = GET(observationDates, i);
            MLET(path, i, j) = parser->findClotureFromDate(date); 
        }
        j++;
    }
}

double Performance::calculPerfMoyenneFinale() {
    return 0.;
}

double calculPerf() {
    return 0.;
}