#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>

using namespace std;

<<<<<<< HEAD
/// \brief Modèle de Black Scholes
=======
/// \brief Parseur de fichier CSV
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
class ParseCsv
{
public:
    string cheminData_; // chemin pour accéder aux données
    PnlMat *marketData_; // matrice des données du sous-jacent

    /**
     * Construit un parseur de csv
     * 
     * @param[in] cheminData est le chemin des données 
<<<<<<< HEAD
     */
    ParseCsv(string cheminData, int size);
=======
     * @param[in] nbDate est le nombre de lignes dans le fichier CSV
     * @param[in] nbAsset correspond au nombre de colonnes dans le CSV
     */
    ParseCsv(string cheminData, int nbDate, int nbAsset);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    /**
     * Détruit le parseur csv
     */
    ~ParseCsv();

    /**
<<<<<<< HEAD
     * 
     */
     string readFileIntoString(const string& path);

    /**
    */
     void getData();
=======
     * Ouvre le fichier donné par path et renvoie son contenu sous forme de string
     * @param[in] path chemin du fichier
     */
    string readFileIntoString(const string& path);

    /**
     * @brief parse le contenu du fichier renvoyé par readFileIntoString
     * 
     */
    void getData();
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

};


