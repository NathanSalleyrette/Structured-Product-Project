#pragma once

<<<<<<< HEAD
#include "Option.hpp"
=======
#include "ProduitDerive.hpp"
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"

/// \brief Monte Carlo
class MonteCarlo
{
private:
    PnlMat *path_; /*! matrice de taille (N+1)*D contenant les trajectoires des sous-jacents */
    PnlMat *shiftPath_; /*! matrice de taille (N+1)*D contenant les trajectoires des sous-jacents, shiftées à partir de t pour le sous-jacent d*/
    PnlMat *past_; /*! trajectoires du passé */
    PnlMat *subPast_; /*! trajectoires du passé, de taille variable */
    PnlVect *vectSt_; /*! vecteur contenant les prix des sous-jacents en t */
    PnlVect *sum_; /*! vecteur contenant la somme des payoff puis vecteur permettant de réaliser des opérations */
    PnlVect *delta_; /*! vecteur contenant les deltas en t_i */
    PnlVect *deltaPrevious_; /*! vecteur contenant les deltas en t_(i-1) */
    PnlVect *stdDevDelta_; /*! vecteur contenant les écart-types de delta */

public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
<<<<<<< HEAD
    Option *opt_; /*! pointeur sur l'option */
=======
    ProduitDerive *prodd_; /*! pointeur sur le ProduitDerive */
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    PnlRng *rng_; /*! pointeur sur le générateur */
    double fdStep_; /*! pas de différence finie */
    int nbSamples_; /*! nombre de tirages Monte Carlo */


    /**
     * Construit un moteur Monte Carlo
     * 
     * @param[in] mod pointeur vers le modèle
<<<<<<< HEAD
     * @param[in] opt pointeur sur l'option
     * @param[in] fdStep pas de différence finie
     * @param[in] nbSamples nombre de tirages Monte Carlo
     */
    MonteCarlo(BlackScholesModel *mod, Option *opt, double fdStep, int nbSamples, PnlRng *rng);
=======
     * @param[in] prodd pointeur sur le ProduitDerive
     * @param[in] fdStep pas de différence finie
     * @param[in] nbSamples nombre de tirages Monte Carlo
     */
    MonteCarlo(BlackScholesModel *mod, ProduitDerive *prodd, double fdStep, int nbSamples, PnlRng *rng);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    /**
     * Détruit le moteur Monte Carlo
     */
    ~MonteCarlo();

    /**
<<<<<<< HEAD
     * Calcule le prix de l'option à la date 0
=======
     * Calcule le prix du ProduitDerive à la date 0
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic écart type de l'estimateur
     */
    void price(double &prix, double &std_dev);

    /**
<<<<<<< HEAD
     * Calcule le prix de l'option à la date t
=======
     * Calcule le prix du ProduitDerive à la date t
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] std_dev contient l'écart type de l'estimateur
     */
    void price(const PnlMat *past, double t, double &prix, double &std_dev);

    /**
<<<<<<< HEAD
     * Calcule le delta de l'option à la date t
=======
     * Calcule le delta du ProduitDerive à la date t
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev);

    /**
<<<<<<< HEAD
     * Calcule le delta de l'option à la date 0
=======
     * Calcule le delta du ProduitDerive à la date 0
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
     *
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
     */
    void delta(PnlVect *delta, PnlVect *std_dev);

    /**
     * Calcule l'erreur de couverture P&L en T
     * 
     * @param[in] nbHedgeDate nombre de pas des observations du marché
     * @param[out] errorHedge résultat de l'erreur de couverture P&L
     * @param[in] marketData matrice qui contient les données du marché
     */
    void pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData);
};


