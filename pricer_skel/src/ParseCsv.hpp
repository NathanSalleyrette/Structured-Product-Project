#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>

using namespace std;

/// \brief Parseur de fichier CSV
class ParseCsv
{
public:
    string cheminData_; // chemin pour accéder aux données
    PnlMat *marketData_; // matrice des données du sous-jacent

    /**
     * Construit un parseur de csv
     * 
     * @param[in] cheminData est le chemin des données 
     * @param[in] nbDate est le nombre de lignes dans le fichier CSV
     * @param[in] nbAsset correspond au nombre de colonnes dans le CSV
     */
    ParseCsv(string cheminData, int nbDate, int nbAsset);

    /**
     * Détruit le parseur csv
     */
    ~ParseCsv();

    /**
     * Ouvre le fichier donné par path et renvoie son contenu sous forme de string
     * @param[in] path chemin du fichier
     */
    string readFileIntoString(const string& path);

    /**
     * @brief parse le contenu du fichier renvoyé par readFileIntoString
     * 
     */
    void getData();

};


