#pragma once

#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

class Date {
    public:
        Date();

        /**
         * @brief Prends une date en paramètre et retourne la prochaine date du calendrier
         * Saute les samedis et dimanches car ce sont des jours où les actions sont non cotés
         * 
         * @param date 
         * @return string 
         */
        static string nextDate(string date);

        /**
         * @brief Retourne la date d'avant 
         * 
         * @param date 
         * @return string 
         */
        static string previousDate(string date);

        /**
         * @brief La date donnée est elle un samedi ou dimanche ?
         * 
         * @param date 
         * @return true 
         * @return false 
         */
        static bool isSaturdayOrSunday(string date);

        /**
         * @brief Retourne une liste de date comprise entre begin_date et end_date
         * Les deux dates extremes sont comprises dans le vecteur
         * Les samedis et dimanches ne sont pas inclus
         * @param begin_date 
         * @param end_date 
         * @return vector<string> 
         */
        static vector<string> getListOfDates(string begin_date, string end_date);
};