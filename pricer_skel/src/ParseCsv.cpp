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

ParseCsv::ParseCsv(string cheminData, int nbAsset) {
    cheminData_ = cheminData;
    lines = countLine();
    columns = nbAsset;
    marketData_ = pnl_mat_create_from_zero(lines, columns);
}

ParseCsv::~ParseCsv(){
    pnl_mat_free(&marketData_);
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

    


}

void ParseCsv::setCheminData(string cheminData) {
    cheminData_ = cheminData;
    lines = countLine();
    resizeMarketData(lines, columns);
}

