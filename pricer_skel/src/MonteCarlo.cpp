#include "MonteCarlo.hpp"
#include <cmath>
#include <ctime>
#include <iostream>

<<<<<<< HEAD
MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, double fdStep, int nbSamples, PnlRng *rng)
{
    mod_ = mod;
    opt_ = opt;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    rng_ = rng;
    path_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_+1, opt_->size_);
    shiftPath_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_+1, opt_->size_);
    vectSt_ = pnl_vect_create_from_zero(opt_->size_);
    sum_ = pnl_vect_create_from_zero(opt_->size_);
    delta_ = pnl_vect_create_from_zero(opt_->size_);
    deltaPrevious_ = pnl_vect_create_from_zero(opt_->size_);
    stdDevDelta_ = pnl_vect_create_from_zero(opt_->size_);
    past_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, opt_->size_);
    subPast_ = pnl_mat_create_from_zero(opt_->nbTimeSteps_ + 1, opt_->size_);
=======
MonteCarlo::MonteCarlo(BlackScholesModel *mod, ProduitDerive *prodd, double fdStep, int nbSamples, PnlRng *rng)
{
    mod_ = mod;
    prodd_ = prodd;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    rng_ = rng;
    path_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
    shiftPath_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
    vectSt_ = pnl_vect_create_from_zero(prodd_->size_);
    sum_ = pnl_vect_create_from_zero(prodd_->size_);
    delta_ = pnl_vect_create_from_zero(prodd_->size_);
    deltaPrevious_ = pnl_vect_create_from_zero(prodd_->size_);
    stdDevDelta_ = pnl_vect_create_from_zero(prodd_->size_);
    past_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
    subPast_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
}

MonteCarlo::~MonteCarlo() {
    pnl_mat_free(&path_);
    pnl_mat_free(&shiftPath_);
    pnl_mat_free(&past_);
    pnl_mat_free(&subPast_);
    pnl_vect_free(&vectSt_);
    pnl_vect_free(&sum_);
    pnl_vect_free(&delta_);
    pnl_vect_free(&deltaPrevious_);
    pnl_vect_free(&stdDevDelta_);
}

void MonteCarlo::price(double &prix, double &std_dev)
{
    double resPayoff;
    double sum = 0;
    double sumSquared = 0;
    double oneOverM = (1/(double)nbSamples_);
    
    for (int i = 0; i < nbSamples_; i++)
    {
<<<<<<< HEAD
        mod_->asset(path_, opt_->T_, opt_->nbTimeSteps_, rng_);
        resPayoff = opt_->payoff(path_);
=======
        //simulation des trajectoires
        mod_->asset(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_);
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
        sum += resPayoff;
        sumSquared += resPayoff * resPayoff;
    }

    double oneOverMTimesSum = oneOverM * sum;
<<<<<<< HEAD

    double varianceEstim = exp(-2 * mod_->r_ * opt_->T_) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    prix = exp(-mod_->r_ * opt_->T_) * oneOverMTimesSum;
    std_dev = sqrt(varianceEstim*oneOverM);

=======
    double varianceEstim = exp(-2 * mod_->r_ * prodd_->T_) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    //actualisation du payoff simulé par MonteCarlo
    prix = exp(-mod_->r_ * prodd_->T_) * oneOverMTimesSum;
    std_dev = sqrt(varianceEstim*oneOverM);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &std_dev)
{
    double resPayoff;
    double sum = 0;
    double sumSquared = 0;
    double oneOverM = (1/(double)nbSamples_);
    
    for (int i = 0; i < nbSamples_; i++)
    {
<<<<<<< HEAD
        mod_->asset(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        resPayoff = opt_->payoff(path_);
=======
        //simulation des trajectoires
        mod_->asset(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past);
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
        sum += resPayoff;
        sumSquared += resPayoff * resPayoff;
    }
    double oneOverMTimesSum = oneOverM * sum;
<<<<<<< HEAD
    double varianceEstim = exp(-2 * mod_->r_ * (opt_->T_-t)) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    prix = exp(-mod_->r_ * (opt_->T_ - t)) * oneOverMTimesSum;
=======
    double varianceEstim = exp(-2 * mod_->r_ * (prodd_->T_-t)) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    //actualisation du payoff simulé par MonteCarlo
    prix = exp(-mod_->r_ * (prodd_->T_ - t)) * oneOverMTimesSum;
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    std_dev = sqrt(varianceEstim*oneOverM);

}

void MonteCarlo::delta(PnlVect *delta, PnlVect *std_dev)
{
    double payoffPlus;
    double payoffMinus;
    double resPayoff;
<<<<<<< HEAD
    double expon = exp(- mod_->r_ * (opt_->T_))/ (2*fdStep_);
    double TOverN = opt_->T_/opt_->nbTimeSteps_;
=======
    double expon = exp(- mod_->r_ * (prodd_->T_))/ (2*fdStep_);
    double TOverN = prodd_->T_/prodd_->nbTimeSteps_;
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    
    pnl_vect_set_zero(std_dev);
    pnl_vect_set_zero(sum_);

    for (int i = 0; i < nbSamples_; i++)
    {
<<<<<<< HEAD
        mod_->asset(path_, opt_->T_, opt_->nbTimeSteps_, rng_); // M simulations

        for (int d=0; d< opt_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, 0, TOverN);
            payoffPlus = opt_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, 0, TOverN);
            payoffMinus = opt_->payoff(shiftPath_);
=======
        //simulation des trajectoires
        mod_->asset(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_); // M simulations

        for (int d=0; d< prodd_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, 0, TOverN);
            payoffPlus = prodd_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, 0, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }
    }

<<<<<<< HEAD
=======
    // ensemble de transformations pour obtenir le delta
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    pnl_vect_clone(vectSt_, mod_->spot_);
    pnl_vect_div_scalar(sum_, ((double)nbSamples_));
    pnl_vect_clone(delta, sum_);
    pnl_vect_mult_scalar(delta, expon);
    pnl_vect_div_vect_term(delta, vectSt_);

    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_mult_vect_term(sum_, sum_);

    pnl_vect_minus_vect(std_dev, sum_);
   
    pnl_vect_mult_scalar(std_dev, expon*expon);
    pnl_vect_mult_vect_term(vectSt_, vectSt_);
    pnl_vect_div_vect_term(std_dev, vectSt_);
    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_map_inplace(std_dev, sqrt);

}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev)
{
    double payoffPlus;
    double payoffMinus;
    double resPayoff;
<<<<<<< HEAD
    double expon = exp(- mod_->r_ * (opt_->T_ - t))/ (2*fdStep_);
    double TOverN = opt_->T_/opt_->nbTimeSteps_;
=======
    double expon = exp(- mod_->r_ * (prodd_->T_ - t))/ (2*fdStep_);
    double TOverN = prodd_->T_/prodd_->nbTimeSteps_;
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    pnl_vect_set_zero(std_dev);
    pnl_vect_set_zero(sum_);
    
    for (int i = 0; i < nbSamples_; i++)
    {
<<<<<<< HEAD
        mod_->asset(path_, t, opt_->T_, opt_->nbTimeSteps_, rng_, past); // M simulations
        for (int d=0; d< opt_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, t, TOverN);
            payoffPlus = opt_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, t, TOverN);
            payoffMinus = opt_->payoff(shiftPath_);
=======
        //simulation des trajectoires
        mod_->asset(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past); // M simulations
        for (int d=0; d< prodd_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, t, TOverN);
            payoffPlus = prodd_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, t, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }
    }

<<<<<<< HEAD
=======
    // ensemble de transformations pour obtenir le delta
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    pnl_mat_get_row(vectSt_, past, past->m-1);
    pnl_vect_div_scalar(sum_, ((double)nbSamples_));
    pnl_vect_clone(delta, sum_);
    pnl_vect_mult_scalar(delta, expon);
    pnl_vect_div_vect_term(delta, vectSt_);

    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_mult_vect_term(sum_, sum_);

    pnl_vect_minus_vect(std_dev, sum_);
   
    pnl_vect_mult_scalar(std_dev, expon*expon);
    pnl_vect_mult_vect_term(vectSt_, vectSt_);
    pnl_vect_div_vect_term(std_dev, vectSt_);
    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_map_inplace(std_dev, sqrt);
}


void MonteCarlo::pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData)
{
    double V;
    double prix, std_dev;

    MonteCarlo::price(prix, std_dev);

    MonteCarlo::delta(deltaPrevious_, stdDevDelta_);
<<<<<<< HEAD
    int HOverN = (int)(nbHedgeDate / opt_->nbTimeSteps_);
    double TOverH = opt_->T_/nbHedgeDate;
    double expon = exp(mod_->r_*TOverH);

    PnlVect vecLine;

    vecLine = pnl_vect_wrap_mat_row(marketData, 0);

    V = prix - pnl_vect_scalar_prod(deltaPrevious_, &vecLine);
=======
    int HOverN = (int)(nbHedgeDate / prodd_->nbTimeSteps_);
    double TOverH = prodd_->T_/nbHedgeDate;
    double expon = exp(mod_->r_*TOverH);

    PnlVect vecLine = pnl_vect_wrap_mat_row(marketData, 0);

    V = prix - pnl_vect_scalar_prod(deltaPrevious_, &vecLine); // /!\ Pour le call vanille on part du prix calculé avec price mais pour Kozei on part du fonds initial !
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    pnl_mat_set_row(past_, &vecLine, 0);

    int pastIndex = 1;
<<<<<<< HEAD
    pnl_mat_resize(subPast_, pastIndex + 1, opt_->size_);
=======
    pnl_mat_resize(subPast_, pastIndex + 1, prodd_->size_);
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5

    vecLine = pnl_vect_wrap_mat_row(marketData, 0);
    pnl_mat_set_row(subPast_,  &vecLine, 0);

    for (int t = 1; t < nbHedgeDate + 1; t++) // chaque t est une date de rebalancement, c'est la grille fine
    {
        vecLine = pnl_vect_wrap_mat_row(marketData, t); // on recupere les données historique de la date t
        if (t % HOverN == 0) // le t est un ti (une date de constatation)
        {
            pnl_mat_set_row(past_, &vecLine, pastIndex);
            pnl_mat_set_row(subPast_, &vecLine, pastIndex);
            pastIndex ++;
        }
        else
        {
            pnl_mat_set_row(subPast_, &vecLine, pastIndex); // on ecrase l'ancienne valeur (l'ancienne date t)
        }

        MonteCarlo::delta(subPast_, t*TOverH, delta_, stdDevDelta_); // on rebalance donc on calcule le delta
        pnl_vect_minus_vect(deltaPrevious_, delta_);
        V = V * expon + pnl_vect_scalar_prod(deltaPrevious_, &vecLine);
        pnl_vect_clone(deltaPrevious_, delta_);

        if (t % HOverN == 0) // le t est un ti
        {
<<<<<<< HEAD
            pnl_mat_resize(subPast_, pastIndex + 1, opt_->size_);
            // pnl_mat_add_row(subPast_, subPast_->m, NULL); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
        }

    errorHedge = V + pnl_vect_scalar_prod(delta_, &vecLine) - opt_->payoff(past_); // calcule du PnL
=======
            pnl_mat_resize(subPast_, pastIndex + 1, prodd_->size_); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
        }

    errorHedge = V + pnl_vect_scalar_prod(delta_, &vecLine) - prodd_->payoff(past_); // calcule du PnL
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    }
}