#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <iostream>


/// \brief abstract class Stock
class Stock
{
public:
    string name; /// name 
    string country; /// country
    double spot; /// current value
    Stock(string name, string country, double spot)
        :name(name)
        ,country(country)
        ,spot(spot)
    { }

    ~Stock() { }

    Stock(const Stock &Sto)
        :name(Sto.name)
        ,country(Sto.country)
        ,spot(Sto.spot)
    { }


};
