#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>

using namespace std;

/// \brief Modèle de Black Scholes
class ParseCsv
{
public:
    string cheminData_; // chemin pour accéder aux données
    PnlMat *marketData_; // matrice des données du sous-jacent

    /**
     * Construit un parseur de csv
     * 
     * @param[in] cheminData est le chemin des données 
     */
    ParseCsv(string cheminData, int size);

    /**
     * Détruit le parseur csv
     */
    ~ParseCsv();

    /**
     * 
     */
     string readFileIntoString(const string& path);

    /**
    */
     void getData();

};


