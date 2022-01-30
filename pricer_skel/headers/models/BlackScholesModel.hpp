#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Modèle de Black Scholes
class BlackScholesModel
{
public:
    int size_; /// nombre d'actifs du modèle
    double r_; /// taux d'intérêt
    double rho_; /// paramètre de corrélation
    PnlVect *sigma_; /// vecteur de volatilités
    PnlVect *spot_; /// valeurs initiales des sous-jacents
    PnlMat *correlations_; /// matrice de correlation des sous-jacents
    PnlVect *G_; /// vecteur pour stocker les vecteurs gaussiens

    /**
     * Construit un modèle BlackScholes
     * 
     * @param[in] size nombre d'actifs du modèle
     * @param[in] r taux d'intérêt
     * @param[in] rho paramètre de corrélation
     * @param[in] sigma vecteur de volatilités
     * @param[in] spot valeurs initiales des sous-jacents
     */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot);

    /**
     * Détruit le modèle BlackScholes
     */
    ~BlackScholesModel();

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] dividende dividende pour la conversion de r en euro
     */
    void assetDelta(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng, PnlVect *dividende);

    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */
    void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     * @param[in] dividende dividende pour la conversion de r en euro
     */
    void assetDelta(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past, PnlVect *dividende);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a été shiftée par (1+h)
     * à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

    /**
     * @brief Simule les données de marché manquantes jusqu'à la maturité (à partir des données fournies dans past)
     * 
     * @param[in] past données du passé disponibles
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] rng 
     * @param[in] trend tendance du modèle
     * @param[in] nbHedgeDate nombre de pas des observations du marché
     * @param[out] path données de past + données simulées
     */
    void simul_market(PnlMat *past, double T, PnlRng *rng, PnlVect *trend, int nbHedgeDate, PnlMat *path);

};


