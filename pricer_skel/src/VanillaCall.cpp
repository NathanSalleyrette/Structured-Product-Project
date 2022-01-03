#include "VanillaCall.hpp"
#include <cmath>

VanillaCall::VanillaCall(double T, int nbTimeSteps, double K)
{
<<<<<<< HEAD
    Option::T_ = T;
    Option::nbTimeSteps_ = nbTimeSteps;
    Option::size_ = 1;
=======
    ProduitDerive::T_ = T;
    ProduitDerive::nbTimeSteps_ = nbTimeSteps;
    ProduitDerive::size_ = 1;
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    K_ = K;
}

double VanillaCall::payoff(const PnlMat *path)
{
    return fmax(MGET(path, nbTimeSteps_, 0) - K_, 0);
}
