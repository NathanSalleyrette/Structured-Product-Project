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

    static double covariance(const PnlVect* X,const PnlVect* Y);
    static double esperance(const PnlVect* X);
    static double correlation(const PnlVect* X, const PnlVect* Y);

    static void correlationMatrix(const PnlMat* path, PnlMat* corrMat);
};