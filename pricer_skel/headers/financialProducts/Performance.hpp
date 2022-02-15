#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include "../fs/ParseYahooCsv.hpp"
#include "../utils/MarketData.hpp"
#include "Derivative.hpp"
using namespace std;

// @brief Calcul de performance pour les 30 actions
class Performance: public Derivative {
    private:
        
        PnlVect *nivInitAct;
        // les dates de constatations (attention, certaines dates ne sont pas encore arrivées)
        vector<string> observationDates;

        // Donnée du marché
        MarketData *md;

        vector<string> simulationDates;

        PnlVect *spotsOnDate;



    public:


        int country_[30];
        Performance(vector<string> observationDates, MarketData *md);
        Performance(vector<string> observationDates, MarketData *md, vector<string> simulationDates);
        Performance(vector<string> observationDates, MarketData *md, int country[]);

        ~Performance();

        double payoff(const PnlMat* path);
        double payoff(PnlMat* path, const PnlMat* changes);
        // Calcul et mets dans le vecteur le niveau initial de chaque action
        void niveauInitial();

        double calculPerfMoyenneFinale(const PnlMat* path);

        double calculPerfDate(string date);

        double calculPerfSemestre(PnlVect *semestre);

        void printObservationDates();

        void setObservationDates(vector<string> od);

        PnlVect *getNivInitAct();
};