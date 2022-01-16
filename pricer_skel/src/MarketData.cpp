#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <filesystem>
#include <sstream>
#include "MarketData.hpp"
#include "fs/ParseYahooCsv.hpp"
#include "utils/Date.hpp"
#include <unistd.h>
#include <math.h>
#include <cassert>
using std::filesystem::directory_iterator;


MarketData::MarketData() {}

map<string, map<string, double>> & MarketData::getData() {
    return data;
}

double MarketData::getSpotFromDateAndAction(string date, string action) {

    while (!(data.find(date) != data.end() && data[date].find(action) != data[date].end())) {
        date = Date::nextDate(date);
    }
    
    return data[date][action];
}

int MarketData::getNumOfActions() { return actions.size(); }


void MarketData::fillData(ParseYahooCsv *parser) {
    string pathFiles = "../data/DATA";
    string action;
    for (const auto & entry : directory_iterator(pathFiles)) {
        action = entry.path(); //pas mieux path().filename().toString() ?
        action.erase(action.end()-4, action.end());
        action.erase(action.begin(), action.begin()+13);
        bool in = false;
        for(int i = 0; i < actions.size(); i++){
            if (actions[i] == action){
                in = true;
            }
        }
        if(!in){
    actions.push_back(action);
        }
            
        

        parser->setCheminData(entry.path());
        parser->fillDictFromFile(data, action);
    }
} 

// Le vecteur doit être de la bonne taille
void MarketData::getSpotsFromDate(PnlVect* spots, string date) {
    vector<string>::iterator it;  
    int i = 0;
    for (it = actions.begin(); it != actions.end(); it++, i++) {
        LET(spots, i) = getSpotFromDateAndAction(date, *it);
    }
}

void MarketData::printActions() {
    vector<string>::iterator it;
    for (it = actions.begin(); it != actions.end(); it++) {
        cout << *it << ' ';
    }
    cout << endl;

}

// int MarketData::getNbStocks(){
//     return actions.size();
// }


void MarketData::fiilPathMat(PnlMat* path, string startDate, int nbOfDays) {
    pnl_mat_resize(path, nbOfDays, actions.size());
    PnlVect* spotsOfDate = pnl_vect_create(actions.size());
    string date = startDate;
    for (int i = 0; i < nbOfDays; i++){
        getSpotsFromDate(spotsOfDate, date);
        pnl_mat_set_row(path, spotsOfDate, i);
        date = Date::nextDate(date);
    }
}

void MarketData::fillfromPath(const PnlMat* path, vector<string> dates){
    
    for(int i = 0; i < dates.size(); i++){
        // for(int i = 0; i < path->m; i++){
        for(int j = 0; j < this->actions.size(); j++){
            // double cours = MGET(path, i,j);
            // if (isnan(cours)) {
            //     std::cout << cours;
            //     std::cout << " i " << i << " j " << j << std::endl;
            //     assert(1==2);
            // }
            
            this->data[dates[i]][this->actions[j]] = MGET(path, i, j);
        }
    }
}

