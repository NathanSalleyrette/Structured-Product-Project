#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include "financialProducts/Performance.hpp"


Performance::Performance(vector<string> observationDates, MarketData *md) {
    this->observationDates = observationDates;
    nivInitAct = pnl_vect_create_from_zero(md->getNumOfActions());
    spotsOnDate = pnl_vect_create_from_zero(md->getNumOfActions());
    pathclone = pnl_mat_create(1, 1);
    pathPerf = pnl_mat_create(observationDates.size() + 1, md->getNumOfActions());
    this->md = md;
}

Performance::Performance(vector<string> observationDates, MarketData *md, vector<string> simulationDates){
    this->observationDates = observationDates;
    nivInitAct = pnl_vect_create_from_zero(md->getNumOfActions());
    spotsOnDate = pnl_vect_create_from_zero(md->getNumOfActions());
    pathclone = pnl_mat_create(1, 1);
    pathPerf = pnl_mat_create(observationDates.size() + 1, md->getNumOfActions());

    this->md = md;
    this->simulationDates = simulationDates;

    // A modifier
    Derivative::T_ = 1;
    Derivative::nbTimeSteps_ = simulationDates.size(); /// nombre de pas de temps de discrétisation (égal 16 dans notre cas)
    Derivative::size_= md->getNumOfActions();
    
}

Performance::Performance(vector<string> observationDates, int nbTimeSteps, MarketData *md, int country[]) {
    this->observationDates = observationDates;
    nivInitAct = pnl_vect_create_from_zero(md->getNumOfActions());
    spotsOnDate = pnl_vect_create_from_zero(md->getNumOfActions());
    pathclone = pnl_mat_create(1, 1);
    pathPerf = pnl_mat_create(observationDates.size() + 1, md->getNumOfActions());

    this->md = md;

    // A modifier
    Derivative::T_ = 1;
    Derivative::nbTimeSteps_ = nbTimeSteps - 1; /// nombre de pas de temps de discrétisation
    Derivative::size_= md->getNumOfActions();
    
    for(int i = 0; i < 30; i++){
        country_[i] = country[i];
    }

    this->md = md;
}

Performance::~Performance() {
    pnl_vect_free(&nivInitAct);
    pnl_vect_free(&spotsOnDate);
    pnl_mat_free(&pathclone);
    pnl_mat_free(&pathPerf);
}

// path est une matrice de taille nbTimeSteps
void Performance::getObservationPath(const PnlMat* path) {
    int jump = (int) ((path->m - 1) / observationDates.size());
    for (int i = 0; i < pathPerf->m; i ++) {
        for (int j = 0; j < pathPerf->n; j++) {
            MLET(pathPerf, i, j) = MGET(path, i*jump, j);
        }
        // pnl_mat_set_row(pathPerf, &pnl_vect_wrap_mat_row(path, i *jump), i);
    }

}

// Calculons le payoff uniquement avec la matrice path
// La ligne 0 étant le spot (ici le niveau initial)
double Performance::payoff(const PnlMat* path) {
    
    getObservationPath(path);
    //md->fillfromPath(path, this->simulationDates);
    //niveauInitial();
    return .9 + calculPerfMoyenneFinale(pathPerf)/100.;
}


double Performance::payoff(PnlMat* path, const PnlMat* changes) {
    
    //md->fillfromPath(path, this->simulationDates);
    //niveauInitial();

    //on remet le bon path en multipliant par l'inverse du taux de change
    //pnl_mat_resize(pathclone, path->m, path->n);

    getObservationPath(path);

    pnl_mat_clone(pathclone, pathPerf);
    for(int i = 0; i < pathPerf->m; i ++){
        for(int j = 0; j < pathPerf->n; j++){

            if (country_[j] !=7){
                MLET(pathclone, i, j) = MGET(pathPerf,i,j) / MGET(changes, i, country_[j]);
                
            }
        }
    }
    double perfMoyenneFinale = calculPerfMoyenneFinale(pathclone);
    return .9 + perfMoyenneFinale/100.;
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

double Performance::calculPerfSemestre(PnlVect *semestre) {
    
    double perfSemestre = 0.;
    double nivInit;
    double perfAction;
    for (int i = 0; i < semestre->size; i++) {
            nivInit = GET(nivInitAct, i);
            perfAction = (GET(semestre, i) - nivInit) / nivInit;
            perfSemestre += perfAction;
    }
    
    return std::max(perfSemestre / semestre->size, 0.);
}


double Performance::calculPerfMoyenneFinale(const PnlMat* path) {

    // double perfMoyenne = 0.;
    // vector<string>::iterator it;
    // for (it = observationDates.begin(); it != observationDates.end(); it++) {
    //         perfMoyenne += calculPerfDate(*it);
    // }
    // return 100*roundFourDecimal(perfMoyenne / observationDates.size());

    double perfMoyenne = 0.;
    // On commence à 1 car la ligne 0 est le spot et donc le niveau initial
    for (int i = 1; i < path->m; i++) {
        pnl_mat_get_row(spotsOnDate, path, i);
        perfMoyenne += calculPerfSemestre(spotsOnDate);
    }

    // On remplace observationDates.size() par path->m - 1 pour enlever sa dépendance
    return 100*roundFourDecimal(perfMoyenne / (path->m - 1));

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

PnlVect *Performance::getNivInitAct() {
    PnlVect* newniveau = pnl_vect_create( nivInitAct->size );
    pnl_vect_clone(newniveau, nivInitAct);
    return newniveau;
}




