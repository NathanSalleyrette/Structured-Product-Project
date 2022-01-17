#include "utils/MarketData.hpp"
#include "fs/ParseYahooCsv.hpp"
#include "utils/Date.hpp"
#include <map>
#include "gtest/gtest.h"
#include "spdlog/log.hpp"

namespace {

    TEST(MarketData, getSpot) {
        log::init();
        ParseYahooCsv *parser = new ParseYahooCsv();
        parser->setCheminData("../../data/DATA/FLS.csv");

        MarketData *market = new MarketData();

        market->fillData(parser);

        cout << market->getSpotFromDateAndAction("2021-11-11", "FLS") << endl;
        EXPECT_DOUBLE_EQ(market->getSpotFromDateAndAction("2021-11-11", "FLS"), 33.509998);

        market->printActions();

        PnlVect *result = pnl_vect_create(market->getNumOfActions());

        market->getSpotsFromDate(result, "2014-07-11");
        pnl_vect_print(result);

        market->getSpotsFromDate(result, "2014-07-15");
        pnl_vect_print(result);

        market->getSpotsFromDate(result, "2014-07-16");

        pnl_vect_print(result);
        pnl_vect_free(&result);
    }

    // Avec ce test, on verifie qu'aucune action n'est côtée un samedi ou un dimanche
    TEST(MarketData, haveSundayOrSaturday) {
        ParseYahooCsv *parser = new ParseYahooCsv();

        MarketData *market = new MarketData();

        market->fillData(parser);
        map<string, map<string, double>> data = market->getData();
        map<string, map<string, double>>::iterator it;
        for (it = data.begin(); it != data.end(); it++) {
            EXPECT_FALSE(Date::isSaturdayOrSunday(it->first));
        }

    }
}