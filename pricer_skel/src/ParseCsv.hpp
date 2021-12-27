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
    PnlVect *dates_;
    int lines;
    int columns;

    /**
     * Construit un parseur de csv
     * 
     * @param[in] nbAsset correspond au nombre de colonnes dans le CSV
     */
    ParseCsv(int nbAsset);

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
    int countLine();
    void resizeMarketData(int line, int col);
    void setCheminData(string cheminData);
    double findClotureFromDate(double date);
};


