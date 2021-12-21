#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Option.hpp"

/// \brief Option vanille

class VanillaCall : public Option
{
public:

    double K_; /// Strike de l'option Vanille

    /**
     * Construit une option vanille
     * 
     * @param[in] T maturité de l'option
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] K strike de l'option
     */
    VanillaCall(double T, int nbTimeSteps, double K);

    /**
     * Détruit l'option vanille
     */
    ~VanillaCall(){}
    
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path);
};