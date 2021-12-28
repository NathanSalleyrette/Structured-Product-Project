#include <iostream>
#include <string>
#include "ParseCsv.hpp"
#include "Performance.hpp"
#include <filesystem>
#include <dirent.h>
#include <vector>

using namespace std;


int main() {
    ParseCsv *parser = new ParseCsv(7);
    parser->setCheminData("../data/DATA/FLS.csv");
    parser->getData();

    parser->findClotureFromDate(20200103.);

    string path = "../data/DATA";
    for (const auto & entry : std::filesystem::directory_iterator(path))
        cout << entry.path() << endl;

    int i = 0;
    delete parser;

    double dates[14] = {20150112, 20150713, 20160111, 20160711, 20170111, 20170711, 20180111
                    , 20180711, 20190111, 20190711, 20200111, 20200711, 20210111, 20210711};
    PnlVect *observeDates = pnl_vect_create_from_ptr(14, dates);
    Performance *perf = new Performance(observeDates, 29);
    perf->niveauInitial();
    perf->setPath();
    double result = perf->calculPerfMoyenneFinale();

    cout << "Performance au 7 Juillet 2021 : " << result << "%" << endl;

    pnl_vect_free(&observeDates);
    delete perf;

    // DIR *dir; struct dirent *diread;
    // vector<char *> files;

    // if ((dir = opendir("../data/DATA/")) != nullptr) {
    //     while ((diread = readdir(dir)) != nullptr) {
    //         files.push_back(diread->d_name);
    //     }
    //     closedir (dir);
    // } else {
    //     perror ("opendir");
    //     return EXIT_FAILURE;
    // }

    // for (auto file : files) cout << file << "| ";
    // cout << endl;

    // return EXIT_SUCCESS;
}