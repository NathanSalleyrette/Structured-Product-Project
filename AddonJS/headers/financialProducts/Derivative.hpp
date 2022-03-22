#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

// @brief abstract Derivative class

class Derivative
{
public:

    double T_; // maturity
    int nbTimeSteps_; // number of steps in the subdivision
    int size_; // model size, (redondant avec BlackScholesModel::size_ ?)
    
    /**
     * @brief Compute the payoff from the path
     *
     * @param[in] path is a (N+1) x d matrix
     * containing the path of the product
     * @return the payoff : phi(path)
     */
    virtual double payoff(const PnlMat *path) = 0;
    virtual double payoff(PnlMat* path, const PnlMat* changes) = 0;
    
    /**
     * @brief free the product
     * 
     */
    virtual ~Derivative(){}
};
