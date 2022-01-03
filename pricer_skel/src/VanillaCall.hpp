#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
<<<<<<< HEAD
#include "Option.hpp"

/// \brief Option vanille

class VanillaCall : public Option
=======
#include "ProduitDerive.hpp"

/// \brief Produit dérivé Option vanille

class VanillaCall : public ProduitDerive
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
{
public:

    double K_; /// Strike de l'option Vanille

    /**
     * Construit une option vanille
     * 
<<<<<<< HEAD
     * @param[in] T maturité de l'option
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] K strike de l'option
=======
     * @param[in] T maturité du ProduitDerive
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] K strike du ProduitDerive
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
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