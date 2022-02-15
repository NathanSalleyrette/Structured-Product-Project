#pragma once


#include "../financialProducts/Derivative.hpp"
#include "../models/BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include<cstdio>

/// \brief Monte Carlo
class MonteCarlo
{
private:
    PnlMat *path_; /*! matrice de taille (N+1)*D contenant les trajectoires des sous-jacents */
    PnlMat *shiftPath_; /*! matrice de taille (N+1)*D contenant les trajectoires des sous-jacents, shiftées à partir de t pour le sous-jacent d*/
    //PnlMat *past_; /*! trajectoires du passé */
    PnlMat* shiftpathchange_;
    //PnlMat *subPast_; /*! trajectoires du passé, de taille variable */
    PnlVect *vectSt_; /*! vecteur contenant les prix des sous-jacents en t */
    PnlVect *sum_; /*! vecteur contenant la somme des payoff puis vecteur permettant de réaliser des opérations */
    //PnlVect *delta_; /*! vecteur contenant les deltas en t_i */
    //PnlVect *deltaPrevious_; /*! vecteur contenant les deltas en t_(i-1) */
    //PnlVect *stdDevDelta_; /*! vecteur contenant les écart-types de delta */
    PnlVect* ratesSum_;
    PnlVect* vectStRate_;

    PnlMat* changesPath_;

public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    BlackScholesModel *modRates_;
    Derivative *prodd_; /*! pointeur sur le Derivative */
    PnlRng *rng_; /*! pointeur sur le générateur */
    double fdStep_; /*! pas de différence finie */
    int nbSamples_; /*! nombre de tirages Monte Carlo */


    /**
     * Construit un moteur Monte Carlo
     * 
     * @param[in] mod pointeur vers le modèle

     * @param[in] prodd pointeur sur le Derivative
     * @param[in] fdStep pas de différence finie
     * @param[in] nbSamples nombre de tirages Monte Carlo
     */
    MonteCarlo(BlackScholesModel *mod, Derivative *prodd, double fdStep, int nbSamples, PnlRng *rng);


    MonteCarlo(BlackScholesModel *mod, Derivative *prodd, double fdStep, int nbSamples, PnlRng *rng, BlackScholesModel *modRates);

    /**
     * Détruit le moteur Monte Carlo
     */
    ~MonteCarlo();

    /**
     * Calcule le prix du Derivative à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic écart type de l'estimateur
     */
    void price(double &prix, double &std_dev);

    /**
     * Calcule le prix du Derivative à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] std_dev contient l'écart type de l'estimateur
     */
    void price(const PnlMat *past, double t, double &prix, double &std_dev);

    /**
     * Calcule le delta du Derivative à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
     * @param[in] dividende dividende pour la conversion de r en euro
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev, PnlVect *dividende);

    /**
     * Calcule le delta du Derivative à la date 0
     *
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
     * @param[in] dividende dividende pour la conversion de r en euro
     */
    void delta(PnlVect *delta, PnlVect *std_dev, PnlVect *dividende);

    /**
     * Calcule l'erreur de couverture P&L en T
     * 
     * @param[in] nbHedgeDate nombre de pas des observations du marché
     * @param[out] errorHedge résultat de l'erreur de couverture P&L
     * @param[in] marketData matrice qui contient les données du marché
     * @param[in] valLiqRef valeur liquidative de référence : valeur dont on dispose au départ pour se couvrir
     */
    void pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData, double valLiqRef);


    /**
     * @brief Calcule l'erreur de couverture P&L en T
     * 
     * @param[in] nbHedgeDate nombre de pas des observations du marché
     * @param[out] errorHedge résultat de l'erreur de couverture P&L
     * @param[in] marketData matrice qui contient les données du marché
     * @param[in] valLiqRef valeur liquidative de référence : valeur dont on dispose au départ pour se couvrir
     * @param pathRates matrice qui contient les données de change
     * @param divStocks vecteur contenant les dividandes pour les actions
     * @param divRates vecteur contenant les dividendes pour les changes
     */
    void pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData, double valLiqRef, PnlMat* pathRates, PnlVect* divStocks, PnlVect* divRates);

    /**
     * Calcule le prix du Derivative à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic écart type de l'estimateur
     * @param divStocks dividende pour les actions
     * @param divRates dividende pour les taux de changes
     */

    void price(double &prix, double &std_dev, PnlVect* divStocks, PnlVect* divRates);


    void price(const PnlMat *past, double t, double &prix, double &std_dev, PnlVect* divStocks, PnlVect* divRates, PnlMat* pastRates);

    void delta(PnlVect *delta, PnlVect *std_dev, PnlVect *dividende, PnlVect* deltachange, PnlVect* divRates, int country[]);

    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev, PnlVect *dividende, PnlVect* deltachange, PnlVect* divRates, int country[], PnlMat* pastRates);

};


