#include "../../headers/utils/Date.hpp"

using namespace std;

int main() {
    string date = "2021-01-11";
    string nextDate = Date::nextDate(date);
    cout << date << endl;
    cout << nextDate << endl;
    cout << "" << endl;
    
    date = "2020-12-31";
    nextDate = Date::nextDate(date);
    cout << date << endl;
    cout << nextDate << endl;
    cout << "" << endl;
}