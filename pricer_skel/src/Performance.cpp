#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include <iostream>
#include <cmath>
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

double roundFourDecimal(double d) {
    return round(d * 10000.0) / 10000.0;
}

double roundTwoDecimal(double d) {
    return round(d * 100.0) / 100.0;
}

double Performance::calculPerfMoyenneFinale() {
    double perfMoyenne = 0.;
    for (int i = 0; i < observationDates->size; i++) {
        perfMoyenne += calculPerfSemestre(i);
    }
    return 100*roundFourDecimal(perfMoyenne / observationDates->size);
}

// Calcul la performance des 30 actions sur un indice donnée
// qui correspond à l'indice de la date souhaitée dans le vecteur observationDates
double Performance::calculPerfSemestre(int ind_dates) {
    double perfSemestre = 0.;
    PnlVect row = pnl_vect_wrap_mat_row(path, ind_dates);
    for (int i = 0; i < row.size; i++) {
        double nivInit = GET(nivInitAct, i);
        double perfAction = (row.array[i] - nivInit) / nivInit;
        perfSemestre += roundFourDecimal(perfAction);
    }
    return roundFourDecimal(std::max(perfSemestre / row.size, 0.));
}

