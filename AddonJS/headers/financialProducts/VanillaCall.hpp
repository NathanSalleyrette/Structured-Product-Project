#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Derivative.hpp"

/// \brief Produit dérivé Option vanille

class VanillaCall : public Derivative
{
public:

    double K_; /// Strike de l'option Vanille

    /**
     * Construit une option vanille
     * 
     * @param[in] T maturité du Derivative
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] K strike du Derivative
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
    double payoff(const PnlMat *path);
    double payoff(PnlMat* path, const PnlMat* changes);
};