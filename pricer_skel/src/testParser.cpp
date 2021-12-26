#include <iostream>
#include <string>
#include "ParseCsv.hpp"

using namespace std;

int main() {
    ParseCsv *parser = new ParseCsv("../data/DATA/FLS.csv", 7);
    parser->getData();

    int i = 0;
    delete parser;
}