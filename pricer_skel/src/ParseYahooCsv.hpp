#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <sstream>
#include <map>

using namespace std;

/// \brief Parseur de fichier CSV téléchargé sur Yahoo Finance
class ParseYahooCsv
{
public:
    string cheminData_; // chemin pour accéder aux données


    /**
     * Construit un parseur de csv
     */
    ParseYahooCsv();

    /**
     * Détruit le parseur csv
     */
    ~ParseYahooCsv();

    /**
     * Ouvre le fichier donné par path et renvoie son contenu sous forme de string
     * @param[in] path chemin du fichier
     */
    string readFileIntoString(const string& path);

    /**
     * @brief parse le contenu du fichier renvoyé par readFileIntoString
     * 
     */
    

    void setCheminData(string cheminData);
    
    /**
     * @brief Rempli un dictionnaire passé en argument 
     * 
     * @param dict Map à remplir
     * @param action Clé voulue pour la map<string, double>
     */
    void fillDictFromFile(map<string, map<string, double>> & dict, string action);
};


