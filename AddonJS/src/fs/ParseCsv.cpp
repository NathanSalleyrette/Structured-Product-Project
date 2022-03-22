#include "fs/ParseCsv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;

ParseCsv::ParseCsv(string cheminData, int nbDate, int nbAsset) {
    cheminData_ = cheminData;
    marketData_ = pnl_mat_create_from_zero(nbDate, nbAsset);
    getData();
}

ParseCsv::~ParseCsv(){
    pnl_mat_free(&marketData_);
}


string ParseCsv::readFileIntoString(const string& path) {
    auto ss = ostringstream();
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

void ParseCsv::getData()
{
    string filename(cheminData_);
    string file_contents;
    char delimiter = ' '; // delimiteur entre les donnees de chaque ligne

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
    string record;

    int counterLine = 0;
    int counterCol;
    while (std::getline(sstream, record)) {
        istringstream line(record);
        counterCol = 0;
        while (std::getline(line, record, delimiter)) {
            MLET(marketData_, counterLine, counterCol) = std::stod(record);
            counterCol ++;
        }
        counterLine += 1;
    }


}
