#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include "../fs/ParseYahooCsv.hpp"
#include "../MarketData.hpp"
#include "ProduitDerive.hpp"
using namespace std;

/// \brief Calcul de performance pour les 30 actions
class Performance: public ProduitDerive {
    private:
        
        PnlVect *nivInitAct;
        // les dates de constatations (attention, certaines dates ne sont pas encore arrivées)
        vector<string> observationDates;

        // Donnée du marché
        MarketData *md;

        vector<string> simulationDates;


    public:
        Performance(vector<string> observationDates, MarketData *md);
        Performance(vector<string> observationDates, MarketData *md, vector<string> simulationDates);

        ~Performance();

        double payoff(const PnlMat* path);

        // Calcul et mets dans le vecteur le niveau initial de chaque action
        void niveauInitial();

        double calculPerfMoyenneFinale();

        double calculPerfDate(string date);

        void printObservationDates();

        void setObservationDates(vector<string> od);

        PnlVect *getNivInitAct();
};