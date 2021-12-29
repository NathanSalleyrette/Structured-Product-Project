#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include <filesystem>


using namespace std;

class Utils
{
private:
    /* data */
public:
    Utils(/* args */);
    ~Utils();

    double covariance(const PnlVect* X,const PnlVect* Y);
    double esperance(const PnlVect* X);
    double correlation(const PnlVect* X, const PnlVect* Y);
};