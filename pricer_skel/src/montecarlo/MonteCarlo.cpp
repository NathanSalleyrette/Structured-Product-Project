#include "montecarlo/MonteCarlo.hpp"
#include <cmath>
#include <ctime>
#include <iostream>
#include "spdlog/log.hpp"

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Derivative *prodd, double fdStep, int nbSamples, PnlRng *rng)
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
    // delta_ = pnl_vect_create_from_zero(prodd_->size_);
    // deltaPrevious_ = pnl_vect_create_from_zero(prodd_->size_);
    // stdDevDelta_ = pnl_vect_create_from_zero(prodd_->size_);
    //past_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
    // subPast_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
}

MonteCarlo::~MonteCarlo() {
    pnl_mat_free(&path_);
    pnl_mat_free(&shiftPath_);
    //pnl_mat_free(&past_);
    // pnl_mat_free(&subPast_);
    pnl_vect_free(&vectSt_);
    pnl_vect_free(&sum_);
    // pnl_vect_free(&delta_);
    // pnl_vect_free(&deltaPrevious_);
    // pnl_vect_free(&stdDevDelta_);
}

void MonteCarlo::price(double &prix, double &std_dev)
{
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    SPDLOG_LOGGER_INFO(_logger, "Starting computing price at t = 0");
    double resPayoff;
    double sum = 0;
    double sumSquared = 0;
    double oneOverM = (1/(double)nbSamples_);
    
    for (int i = 0; i < nbSamples_; i++)
    {

        //simulation des trajectoires
        mod_->asset(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_);
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_);

        sum += resPayoff;
        
        sumSquared += resPayoff * resPayoff;
        // std::cout<<resPayoff<<std::endl;
    }

    double oneOverMTimesSum = oneOverM * sum;

    double varianceEstim = exp(-2 * mod_->r_ * prodd_->T_) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    //actualisation du payoff simulé par MonteCarlo
    prix = exp(-mod_->r_ * prodd_->T_) * oneOverMTimesSum;
    std_dev = sqrt(varianceEstim*oneOverM);
    SPDLOG_LOGGER_INFO(_logger, "Price at t = 0 computed");
}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &std_dev)
{
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    SPDLOG_LOGGER_INFO(_logger, "Starting computing price at t");
    double resPayoff;
    double sum = 0;
    double sumSquared = 0;
    double oneOverM = (1/(double)nbSamples_);
    
    for (int i = 0; i < nbSamples_; i++)
    {

        //simulation des trajectoires
        mod_->asset(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past);
        // pnl_mat_print(path_);
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_);
        // std::cout << resPayoff << std::endl;
        sum += resPayoff;
        //SPDLOG_LOGGER_INFO(_logger, sum);

        sumSquared += resPayoff * resPayoff;
    }
    double oneOverMTimesSum = oneOverM * sum;

    double varianceEstim = exp(-2 * mod_->r_ * (prodd_->T_-t)) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    //actualisation du payoff simulé par MonteCarlo
    prix = exp(-mod_->r_ * (prodd_->T_ - t)) * oneOverMTimesSum;
    std_dev = sqrt(varianceEstim*oneOverM);
    SPDLOG_LOGGER_INFO(_logger, "Price at t computed");
}

void MonteCarlo::delta(PnlVect *delta, PnlVect *std_dev)
{
    double payoffPlus;
    double payoffMinus;
    double resPayoff;

    double expon = exp(- mod_->r_ * (prodd_->T_))/ (2*fdStep_);
    double TOverN = prodd_->T_/prodd_->nbTimeSteps_;
    
    pnl_vect_set_zero(std_dev);
    pnl_vect_set_zero(sum_);

    for (int i = 0; i < nbSamples_; i++)
    {

        //simulation des trajectoires
        mod_->asset(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_); // M simulations

        for (int d=0; d< prodd_->size_; d++)
        {
            
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, 0, TOverN);
            
            payoffPlus = prodd_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, 0, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_);
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }
    }

    // ensemble de transformations pour obtenir le delta
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

    double expon = exp(- mod_->r_ * (prodd_->T_ - t))/ (2*fdStep_);
    double TOverN = prodd_->T_/prodd_->nbTimeSteps_;

    pnl_vect_set_zero(std_dev);
    pnl_vect_set_zero(sum_);
    
    for (int i = 0; i < nbSamples_; i++)
    {
        //simulation des trajectoires
        mod_->asset(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past); // M simulations
        for (int d=0; d< prodd_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, t, TOverN);
            payoffPlus = prodd_->payoff(shiftPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, t, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_);
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }
    }


    // ensemble de transformations pour obtenir le delta
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


void MonteCarlo::pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData, double valLiqRef)
{

    PnlVect *delta = pnl_vect_create(marketData->n);/*! vecteur contenant les deltas en t_i */
    PnlVect *deltaPrevious = pnl_vect_create(marketData->n); /*! vecteur contenant les deltas en t_(i-1) */
    PnlVect *stdDevDelta = pnl_vect_create(marketData->n); /*! vecteur contenant les écart-types de delta */
    PnlMat *past = pnl_mat_create(prodd_->nbTimeSteps_ + 1, marketData->n); /*! trajectoires du passé */
    PnlMat *subPast = pnl_mat_create(1, marketData->n); /*! trajectoires du passé, de taille variable */


    double V = 0.;
    //int s = path_->n;
    MonteCarlo::delta(deltaPrevious, stdDevDelta);
    int HOverN = (int)(nbHedgeDate / prodd_->nbTimeSteps_);
    int TOverN = (int) prodd_->T_ * (marketData->m - 1) / prodd_->nbTimeSteps_;
    double TOverH = prodd_->T_/nbHedgeDate;
    double expon = exp(mod_->r_*TOverH);


    PnlVect vecLine = pnl_vect_wrap_mat_row(marketData, 0);

    V = valLiqRef - pnl_vect_scalar_prod(deltaPrevious, &vecLine);
    

    pnl_mat_set_row(past, &vecLine, 0);

    int pastIndex = 1;

    pnl_mat_resize(subPast, pastIndex + 1, prodd_->size_);

    // On a commenté car c'est la même chose que la ligne 215
    //vecLine = pnl_vect_wrap_mat_row(marketData, 0);
     
    pnl_mat_set_row(subPast,  &vecLine, 0);

    for (int tbrut = 1; tbrut < nbHedgeDate + 1; tbrut++) // chaque t est une date de rebalancement, c'est la grille fine
    {
        // Si on rebalance a chaque date données le marketData, alors tbrut = t;
        int t = (int) (tbrut*(marketData->m - 1) / nbHedgeDate);

        //std::cout << "t = " << t << std::endl;
        
        // Attention Vigilance
        vecLine = pnl_vect_wrap_mat_row(marketData, t); // on recupere les données historique de la date t
        if (t % TOverN == 0) // le t est un ti (une date de constatation)
        {
            pnl_mat_set_row(past, &vecLine, pastIndex);
        }

        pnl_mat_set_row(subPast, &vecLine, pastIndex);
        
        MonteCarlo::delta(subPast, tbrut*TOverH, delta, stdDevDelta); // on rebalance donc on calcule le delta
        pnl_vect_minus_vect(deltaPrevious, delta);
        V = V * expon + pnl_vect_scalar_prod(deltaPrevious, &vecLine);
        pnl_vect_clone(deltaPrevious, delta);

        if (t % TOverN == 0 && subPast->m < past->m) // le t est un ti, le nombre de ligne de subPast doit être strictement inférieur
        // a celui de past, sinon cela veut dire qu'on est à t=T;
        {
            // Problème ici, car lorsqu'on resize, toutes les données de subPast sont perdus
            pastIndex ++;
            pnl_mat_extract_subblock(subPast, past, 0, pastIndex + 1, 0, prodd_->size_); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
        }

    }

    errorHedge = V + pnl_vect_scalar_prod(delta, &vecLine) - prodd_->payoff(past); // calcule du PnL
    pnl_vect_free(&delta);
    pnl_vect_free(&deltaPrevious);
    pnl_vect_free(&stdDevDelta);
    pnl_mat_free(&past);
    pnl_mat_free(&subPast);
}