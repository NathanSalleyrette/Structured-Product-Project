#include "ParseCsv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;

<<<<<<< HEAD


ParseCsv::ParseCsv(string cheminData, int size) {
    cheminData_ = cheminData;
    marketData_ = pnl_mat_create_from_zero(size, 1);
    getData();
}

string ParseCsv::readFileIntoString(const string& path) {
    auto ss = ostringstream{};
=======
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
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
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
<<<<<<< HEAD
    // std::map<int, std::vector<string>> csv_contents;
    char delimiter = ';';
=======
    char delimiter = ' '; // delimiteur entre les donnees de chaque ligne
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
<<<<<<< HEAD
    std::vector<string> items;
    string record;

    int counter = 0;
    while (std::getline(sstream, record)) {
        istringstream line(record);
        while (std::getline(line, record, delimiter)) {
            // record.erase(std::remove_if(record.begin(), record.end(), isspace), record.end());
            items.push_back(record);
        }

        // csv_contents[counter] = items;
        // for () {

        // }
        MLET(marketData_, counter, 0) = std::stod(items[0]); // on part du principe qu'on a qu'une action sous-jacente donc une seule colonne dans le csv
        items.clear();
        counter += 1;
=======
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
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    }


}
