#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>

using namespace std;

/// \brief Calcul de performance pour les 30 actions
class Performance {
    private:
        ParseCsv *parser;
        // actions en colonne, le temps est en ligne, ce sont les valeurs des actions aux dates de constatations
        PnlMat *path;
        // niveau initial des actions 
        PnlVect *nivInitAct;
        // les dates de constatations (attention, certaines dates ne sont pas encore arrivées)
        PnlVect *observationDates;

    public:
        Performance();

        double payoff();

        // Calcul et mets dans le vecteur le niveau initial de chaque action
        void niveauInitial();

        // met dans la matrice path les bonnes valeurs de clotûres aux bonnes dates
        void setPath();

        double calculPerfMoyenneFinale();

        double calculPerf();
}