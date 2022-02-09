#include "financialProducts/VanillaCall.hpp"
#include <cmath>

VanillaCall::VanillaCall(double T, int nbTimeSteps, double K)
{
    Derivative::T_ = T;
    Derivative::nbTimeSteps_ = nbTimeSteps;
    Derivative::size_ = 1;
    K_ = K;
}

double VanillaCall::payoff(const PnlMat *path)
{
    return fmax(MGET(path, nbTimeSteps_, 0) - K_, 0);
}

double VanillaCall::payoff(PnlMat* path, const PnlMat* changes) {
return fmax(MGET(path, nbTimeSteps_, 0) - K_, 0);
};
