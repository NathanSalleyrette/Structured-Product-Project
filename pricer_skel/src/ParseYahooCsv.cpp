#include "ParseYahooCsv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;

ParseYahooCsv::ParseYahooCsv() {}

ParseYahooCsv::~ParseYahooCsv(){

}

string ParseYahooCsv::readFileIntoString(const string& path) {
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


void ParseYahooCsv::setCheminData(string cheminData) {
    cheminData_ = cheminData;
}



void ParseYahooCsv::fillDictFromFile(map<string, map<string, double>>& dict, string action) {
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
            // Colonne écrit en dur, à changer à l'avenir
            if (counterCol == 4) {
                spot = std::stod(record);
            }
            counterCol ++;
        }
        dict[date].insert(make_pair(action, spot));
        counterLine += 1;
    }

}


