#include "ParseCsv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;



ParseCsv::ParseCsv(string cheminData, int size) {
    cheminData_ = cheminData;
    marketData_ = pnl_mat_create_from_zero(size, 1);
    getData();
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

void ParseCsv::getData()
{
    string filename(cheminData_);
    string file_contents;
    // std::map<int, std::vector<string>> csv_contents;
    char delimiter = ';';

    file_contents = readFileIntoString(filename);

    istringstream sstream(file_contents);
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
    }


}
