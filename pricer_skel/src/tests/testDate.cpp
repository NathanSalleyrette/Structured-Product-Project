#include "../../headers/utils/Date.hpp"
#include "gtest/gtest.h"

namespace
{
    TEST(Date, nextDate)
    {
        // Lundi
        string date = "2021-01-04";
        string nextDate = Date::nextDate(date);
        EXPECT_EQ(nextDate, "2021-01-05");
        // Mardi
        nextDate = Date::nextDate(nextDate);
        EXPECT_EQ(nextDate, "2021-01-06");
        // Mercredi
        nextDate = Date::nextDate(nextDate);
        EXPECT_EQ(nextDate, "2021-01-07");
        // Jeudi -> donne Vendredi
        nextDate = Date::nextDate(nextDate);
        EXPECT_EQ(nextDate, "2021-01-08");
        //Vendredi -> donne Lundi
        nextDate = Date::nextDate(nextDate);
        EXPECT_EQ(nextDate, "2021-01-11");

        //Samedi -> donne Lundi
        nextDate = Date::nextDate("2021-01-09");
        EXPECT_EQ(nextDate, "2021-01-11");
        
        // Dimanche -> donne Lundi
        nextDate = Date::nextDate("2021-01-10");
        EXPECT_EQ(nextDate, "2021-01-11");

    }

    TEST(Date, PreviousDate)
    {
        // Lundi -> donne Vendredi
        string date = "2021-01-04";
        string previousDate = Date::previousDate(date);
        EXPECT_EQ(previousDate, "2021-01-01");
        // Vendredi -> donne Jeudi
        previousDate = Date::previousDate(previousDate);
        EXPECT_EQ(previousDate, "2020-12-31");
        // Jeudi -> donne Mercredi
        previousDate = Date::previousDate(previousDate);
        EXPECT_EQ(previousDate, "2020-12-30");
        // Mercredi -> donne Mardi
        previousDate = Date::previousDate(previousDate);
        EXPECT_EQ(previousDate, "2020-12-29");
        //Mardi -> donne Lundi
        previousDate = Date::previousDate(previousDate);
        EXPECT_EQ(previousDate, "2020-12-28");

        //Samedi -> donne Vendredi
        previousDate = Date::previousDate("2020-12-26");
        EXPECT_EQ(previousDate, "2020-12-25");
        
        // Dimanche -> donne Vendredi
        previousDate = Date::previousDate("2020-12-27");
        EXPECT_EQ(previousDate, "2020-12-25");
    }

    TEST(Date, ListOfDate) {
        vector<string> list;
        list = Date::getListOfDates("2021-01-04", "2021-01-11");
        EXPECT_EQ(list.size(), 6);

    }

}
