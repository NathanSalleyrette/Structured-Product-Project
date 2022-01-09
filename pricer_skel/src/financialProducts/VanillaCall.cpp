#include "financialProducts/VanillaCall.hpp"
#include <cmath>

VanillaCall::VanillaCall(double T, int nbTimeSteps, double K)
{
    ProduitDerive::T_ = T;
    ProduitDerive::nbTimeSteps_ = nbTimeSteps;
    ProduitDerive::size_ = 1;
    K_ = K;
}

double VanillaCall::payoff(const PnlMat *path)
{
    return fmax(MGET(path, nbTimeSteps_, 0) - K_, 0);
}
