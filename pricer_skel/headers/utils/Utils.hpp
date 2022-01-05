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

    /**
     * @brief compute volatility for each stock from spots on a window
     * 
     * @param volatilities the vector to modify with the computation
     * @param path the window on which the computation is made
     */
    static void volsOnMat(PnlVect* volatilities, const PnlMat* path);
};