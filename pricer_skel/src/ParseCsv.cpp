#include "ParseCsv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;

ParseCsv::ParseCsv(int nbAsset) {
    columns = nbAsset;
    marketData_ = pnl_mat_create_from_zero(1, columns);
    dates_ = pnl_vect_new();
}

ParseCsv::~ParseCsv(){
    pnl_mat_free(&marketData_);
    pnl_vect_free(&dates_);
}

void ParseCsv::resizeMarketData(int line, int col) {
    pnl_mat_resize(marketData_, line, col);
}

string ParseCsv::readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int ParseCsv::countLine() {
    string filename(cheminData_);
    string file_contents;

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
    string record;
    int counterLine = 0;

    while (std::getline(sstream, record)) {
        counterLine+=1;
    }
    // -1 car la première ligne est l'entete
    return counterLine - 1;
}

void ParseCsv::getData()
{
    string filename(cheminData_);
    string file_contents;
    // LIGNE CHANGEE CAR MES CSV SONT SEPARES PAR DES VIRGULES
    char delimiter = ','; // delimiteur entre les donnees de chaque ligne

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
    string record;

    int counterLine = 0;
    std::getline(sstream, record);
    int counterCol;
    while (std::getline(sstream, record)) {
        istringstream line(record);
        counterCol = 0;
        while (std::getline(line, record, delimiter)) {
            if (!counterCol) {
                record.erase(std::remove(record.begin(), record.end(), '-'), record.end());
            }
            MLET(marketData_, counterLine, counterCol) = std::stod(record);
            counterCol ++;
        }
        counterLine += 1;
    }
    pnl_mat_get_col(dates_, marketData_, 0);
    


}

void ParseCsv::setCheminData(string cheminData) {
    cheminData_ = cheminData;
    lines = countLine();
    resizeMarketData(lines, columns);
    pnl_vect_resize(dates_, lines);
}

// int ismore ( double *t ) { return t[0] == t[1]; }

// Les dates trop grandes ne sont pas encore prise en compte, il faudra voir comment faire
double ParseCsv::findClotureFromDate(double date) {
    // PnlVectInt *ind = pnl_vect_int_create(1);
    // char p[] = {'r', 'v'};
    // pnl_vect_find(ind, p, ismore, date, dates_);
    if (date > GET(dates_, dates_->size - 1)) return 0.;
    int ind = 0;
    double debut = GET(dates_, ind);
    while (GET(dates_, ind) < date) ind++;
    double result = MGET(marketData_, ind, 4);
    return result;

}

void ParseCsv::fillDictFromFile(map<string, map<string, double>>& dict, string action) {
    string filename(cheminData_);
    string file_contents;
    char delimiter = ','; // delimiteur entre les donnees de chaque ligne

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
    string record;

    int counterLine = 0;
    std::getline(sstream, record);
    int counterCol;
    string date;
    double spot;
    while (std::getline(sstream, record)) {
        istringstream line(record);
        counterCol = 0;
        while (std::getline(line, record, delimiter)) {
            if (counterCol == 0) {
                date = record;
            }
            if (counterCol == 4) {
                spot = std::stod(record);
            }
            // if (!counterCol) {
            //     record.erase(std::remove(record.begin(), record.end(), '-'), record.end());
            // }
            // MLET(marketData_, counterLine, counterCol) = std::stod(record);
            counterCol ++;
        }
        dict[date].insert(make_pair(action, spot));
        counterLine += 1;
    }

}


