#include "montecarlo/MonteCarlo.hpp"
#include <cmath>
#include <ctime>
#include <iostream>
#include "spdlog/log.hpp"
#include "utils/MarketData.hpp"

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

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Derivative *prodd, double fdStep, int nbSamples, PnlRng *rng, MarketData *market)
{
    mod_ = mod;
    prodd_ = prodd;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    rng_ = rng;
    path_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
    market_ = market;
    
    shiftPath_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
    vectSt_ = pnl_vect_create_from_zero(prodd_->size_);
    sum_ = pnl_vect_create_from_zero(prodd_->size_);

    // delta_ = pnl_vect_create_from_zero(prodd_->size_);
    // deltaPrevious_ = pnl_vect_create_from_zero(prodd_->size_);
    // stdDevDelta_ = pnl_vect_create_from_zero(prodd_->size_);
    //past_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
    // subPast_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_ + 1, prodd_->size_);
}


MonteCarlo::MonteCarlo(BlackScholesModel *mod, Derivative *prodd, double fdStep, int nbSamples, PnlRng *rng, BlackScholesModel *modRates)
{
    mod_ = mod;
    prodd_ = prodd;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    rng_ = rng;
    path_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
                   
    shiftPath_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, prodd_->size_);
    modRates_ = modRates;
    changesPath_ = pnl_mat_create_from_zero(prodd_->nbTimeSteps_+1, modRates->size_);
    shiftpathchange_ = pnl_mat_create(prodd_->nbTimeSteps_+1, modRates->size_);

    vectSt_ = pnl_vect_create_from_zero(prodd_->size_);
    vectStRate_ = pnl_vect_create_from_zero(modRates->size_);
    sum_ = pnl_vect_create_from_zero(prodd_->size_);
    ratesSum_ = pnl_vect_create_from_zero(modRates_->size_);
    
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

void MonteCarlo::delta(PnlVect *delta, PnlVect *std_dev, PnlVect *dividende)
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
        mod_->assetDelta(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_, dividende); // M simulations

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

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev, PnlVect *dividende)
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
        mod_->assetDelta(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past, dividende); // M simulations
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
    FILE * f;
    f = fopen ("Pvalue.txt", "wt");
    if (f == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * fp;
    fp = fopen ("Payoff.txt", "wt");
    if (fp == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * Pdates;
    Pdates = fopen ("dates.txt", "wt");
    if (Pdates == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    double V = 0.;
    double prix = 0.;
    double std_dev = 0.;

    // creation dividende à 0 car sans dividendes
    PnlVect *div = pnl_vect_create_from_zero(marketData->n);

    //int s = path_->n;
    MonteCarlo::delta(deltaPrevious, stdDevDelta, div);
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

    // création de la matrice pour la fenetre glissante
    PnlMat* pathWindow = pnl_mat_create(1,1);

    for (int tbrut = 1; tbrut < nbHedgeDate + 1; tbrut++) // chaque t est une date de rebalancement, c'est la grille fine
    {
        // Si on rebalance a chaque date données le marketData, alors tbrut = t;
        int t = (int) (tbrut*(marketData->m - 1) / nbHedgeDate);

        std::cout << "t = " << t << std::endl;
        
        // Attention Vigilance
        vecLine = pnl_vect_wrap_mat_row(marketData, t); // on recupere les données historique de la date t
        if (t % TOverN == 0) // le t est un ti (une date de constatation)
        {
            pnl_mat_set_row(past, &vecLine, pastIndex);
        }

        pnl_mat_set_row(subPast, &vecLine, pastIndex);
        
        MonteCarlo::delta(subPast, tbrut*TOverH, delta, stdDevDelta, div); // on rebalance donc on calcule le delta
        pnl_vect_minus_vect(deltaPrevious, delta);
        V = V * expon + pnl_vect_scalar_prod(deltaPrevious, &vecLine);
        pnl_vect_clone(deltaPrevious, delta);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // On compare ici la valeur du portefeuille à l'instant t avec le prix donné par le pricer
        // Ce seront les données à afficher dans notre application pour le portefeuille de couverture
       
        MonteCarlo::price(subPast, tbrut*TOverH, prix, std_dev);
        double valeurPort = V + pnl_vect_scalar_prod(delta, &vecLine);

        std::cout << "En t = " << tbrut*TOverH << " V = " << valeurPort << " prix  = " << prix << endl;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fprintf(f, "%lf \n", valeurPort);
        fprintf(fp, "%lf \n", prix);
        fprintf(Pdates, "%lf \n", tbrut *TOverH);
        if (t % TOverN == 0 && subPast->m < past->m) // le t est un ti, le nombre de ligne de subPast doit être strictement inférieur
        // a celui de past, sinon cela veut dire qu'on est à t=T;
        {
            // Problème ici, car lorsqu'on resize, toutes les données de subPast sont perdus
            pastIndex ++;
            pnl_mat_extract_subblock(subPast, past, 0, pastIndex + 1, 0, prodd_->size_); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
        }

    }
    fclose(f);
    fclose(fp);
    fclose(Pdates);

    errorHedge = V + pnl_vect_scalar_prod(delta, &vecLine) - prodd_->payoff(past); // calcule du PnL
    pnl_vect_free(&delta);
    pnl_vect_free(&deltaPrevious);
    pnl_vect_free(&stdDevDelta);
    pnl_mat_free(&past);
    pnl_mat_free(&subPast);
}

// PnL avec Fenetre glissante
void MonteCarlo::pAndLWindow(int nbHedgeDate, double &errorHedge, PnlMat *marketData, double valLiqRef, int windowSize, vector<string> rebalancingDates)
{

    PnlVect *delta = pnl_vect_create(marketData->n);/*! vecteur contenant les deltas en t_i */
    PnlVect *deltaPrevious = pnl_vect_create(marketData->n); /*! vecteur contenant les deltas en t_(i-1) */
    PnlVect *stdDevDelta = pnl_vect_create(marketData->n); /*! vecteur contenant les écart-types de delta */
    PnlMat *past = pnl_mat_create(prodd_->nbTimeSteps_ + 1, marketData->n); /*! trajectoires du passé */
    PnlMat *subPast = pnl_mat_create(1, marketData->n); /*! trajectoires du passé, de taille variable */
    FILE * f;
    f = fopen ("Pvalue.txt", "wt");
    if (f == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * fp;
    fp = fopen ("Payoff.txt", "wt");
    if (fp == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * Pdates;
    Pdates = fopen ("dates.txt", "wt");
    if (Pdates == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    double V = 0.;
    double prix = 0.;
    double std_dev = 0.;

    // creation dividende à 0 car sans dividendes
    PnlVect *div = pnl_vect_create_from_zero(marketData->n);

    //int s = path_->n;
    MonteCarlo::delta(deltaPrevious, stdDevDelta, div);
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

    // création de la matrice pour la fenetre glissante
    PnlMat* pathWindow = pnl_mat_create(1,1);

    vector<string> listDatesWindow;
    PnlMat* corrMat = pnl_mat_create(market_->getNumOfActions(), market_->getNumOfActions());

    for (int tbrut = 1; tbrut < nbHedgeDate + 1; tbrut++) // chaque t est une date de rebalancement, c'est la grille fine
    {


        listDatesWindow = Date::getListOfDates("2014-07-11", rebalancingDates[tbrut - 1]);
        
        if (listDatesWindow.size() > windowSize) { // on a besoin d'avoir assez de données dans le passé pour commencer à faire la fenetre glissante
            // Fenetre glissante volatilités et corrélation

            market_->fillPathMatFromFinalDate(pathWindow, rebalancingDates[tbrut - 1], windowSize);

            // volatilité
            // PnlVect* volatilities = pnl_vect_create(market_->getNumOfActions());
            Utils::volsOnMat(mod_->sigma_, pathWindow);
            pnl_vect_mult_scalar(mod_->sigma_, 1. / 16.); // pq 1/16 ??

            // pathWindow = pnl_mat_create_from_scalar(pathWindow->m, pathWindow->n, 2.0);

            // correlation
            Utils::correlationMatrix(pathWindow, corrMat);

            pnl_mat_mult_double(corrMat, 1.);
            pnl_mat_chol(corrMat);
            mod_->correlations_ = corrMat;

            
        }
        
        // Si on rebalance a chaque date données le marketData, alors tbrut = t;
        int t = (int) (tbrut*(marketData->m - 1) / nbHedgeDate);
        
        // Attention Vigilance
        vecLine = pnl_vect_wrap_mat_row(marketData, t); // on recupere les données historique de la date t
        if (t % TOverN == 0) // le t est un ti (une date de constatation)
        {
            pnl_mat_set_row(past, &vecLine, pastIndex);
        }

        pnl_mat_set_row(subPast, &vecLine, pastIndex);
        
        MonteCarlo::delta(subPast, tbrut*TOverH, delta, stdDevDelta, div); // on rebalance donc on calcule le delta
        pnl_vect_minus_vect(deltaPrevious, delta);
        V = V * expon + pnl_vect_scalar_prod(deltaPrevious, &vecLine);
        pnl_vect_clone(deltaPrevious, delta);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // On compare ici la valeur du portefeuille à l'instant t avec le prix donné par le pricer
        // Ce seront les données à afficher dans notre application pour le portefeuille de couverture
       
        MonteCarlo::price(subPast, tbrut*TOverH, prix, std_dev);
        double valeurPort = V + pnl_vect_scalar_prod(delta, &vecLine);

        std::cout << "En t = " << tbrut*TOverH << " V = " << valeurPort << " prix  = " << prix << endl;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fprintf(f, "%lf \n", valeurPort);
        fprintf(fp, "%lf \n", prix);
        fprintf(Pdates, "%lf \n", tbrut *TOverH);


        if (t % TOverN == 0 && subPast->m < past->m) // le t est un ti, le nombre de ligne de subPast doit être strictement inférieur
        // a celui de past, sinon cela veut dire qu'on est à t=T;
        {
            // Problème ici, car lorsqu'on resize, toutes les données de subPast sont perdus
            pastIndex ++;
            pnl_mat_extract_subblock(subPast, past, 0, pastIndex + 1, 0, prodd_->size_); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
        }
    }

    fclose(f);
    fclose(fp);
    fclose(Pdates);

    errorHedge = V + pnl_vect_scalar_prod(delta, &vecLine) - prodd_->payoff(past); // calcule du PnL
    pnl_vect_free(&delta);
    pnl_vect_free(&deltaPrevious);
    pnl_vect_free(&stdDevDelta);
    pnl_mat_free(&past);
    pnl_mat_free(&subPast);
}





//////////////////////////////////////////////////////////////////////////////////////////
// On passe maintenant aux fonctions de prix et delta prenant en compte les taux de changes
////////////////////////////////////////////////////////////////////////////////////////////

void MonteCarlo::price(double &prix, double &std_dev, PnlVect* divStocks, PnlVect* divRates)
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

        mod_->assetDelta(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_, divStocks);

        modRates_->assetDelta(changesPath_, prodd_->T_, prodd_->nbTimeSteps_, rng_, divRates);
        
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_, changesPath_);

        sum += resPayoff;
        
        sumSquared += resPayoff * resPayoff;
    }

    double oneOverMTimesSum = oneOverM * sum;

    double varianceEstim = exp(-2 * mod_->r_ * prodd_->T_) * (oneOverM * sumSquared - oneOverMTimesSum * oneOverMTimesSum);

    //actualisation du payoff simulé par MonteCarlo
    prix = exp(-mod_->r_ * prodd_->T_) * oneOverMTimesSum;
    std_dev = sqrt(varianceEstim*oneOverM);
    SPDLOG_LOGGER_INFO(_logger, "Price at t = 0 computed");
}




void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &std_dev, PnlVect* divStocks, PnlVect* divRates, PnlMat* pastRates)
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
        mod_->assetDelta(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past, divStocks);
        modRates_->assetDelta(changesPath_,t, prodd_->T_, prodd_->nbTimeSteps_, rng_, pastRates ,divRates);

        // pnl_mat_print(path_);
        //payoff pour la trajectoire simulée
        resPayoff = prodd_->payoff(path_, changesPath_);
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


void MonteCarlo::delta(PnlVect *delta, PnlVect *std_dev, PnlVect *dividende, PnlVect* deltachange, PnlVect* divRates, int country[])
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
        mod_->assetDelta(path_, prodd_->T_, prodd_->nbTimeSteps_, rng_, dividende); // M simulations
        modRates_->assetDelta(changesPath_, prodd_->T_, prodd_->nbTimeSteps_, rng_, divRates);
        for (int d=0; d< prodd_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, 0, TOverN);
            
            payoffPlus = prodd_->payoff(shiftPath_, changesPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, 0, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_, changesPath_);

           
            
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }

        for (int d=0; d< modRates_->size_; d++)
        {
            
            modRates_->shiftAsset(shiftpathchange_ , changesPath_ , d, fdStep_, 0, TOverN);
            
            payoffPlus = prodd_->payoff(path_, shiftpathchange_);

          
            mod_->shiftAsset(shiftpathchange_, changesPath_, d, -fdStep_, 0, TOverN);
           
           
            payoffMinus = prodd_->payoff(path_, shiftpathchange_);
            
            resPayoff = payoffPlus - payoffMinus;
            //sstd::cout << resPayoff<< std::endl;
            LET( ratesSum_ , d) += resPayoff; // M addition
            //LET(std_dev, d) += resPayoff * resPayoff;
        }

    }
            
    // ensemble de transformations pour obtenir le delta
    pnl_vect_clone(vectSt_, mod_->spot_);
    pnl_vect_div_scalar(sum_, ((double)nbSamples_));
    pnl_vect_clone(delta, sum_);
    pnl_vect_mult_scalar(delta, expon);
    pnl_vect_div_vect_term(delta, vectSt_);

     
    //On calcul les delta pour les changes
    pnl_vect_clone(vectStRate_, modRates_->spot_);
    pnl_vect_div_scalar(ratesSum_ , ((double)nbSamples_));
    pnl_vect_clone( deltachange , ratesSum_);
    pnl_vect_mult_scalar(deltachange, expon);
    pnl_vect_div_vect_term(deltachange, vectStRate_);

    // for(int i = 0; i < mod_->size_; i++){
    //     if(country[i]!= 7)
    //     LET(deltachange, country[i]) = +GET(deltachange, country[i]) + GET(delta, i)/GET(modRates_->spot_, country[i])*GET(mod_->spot_, i);
    // }


    //calcul erreur pour test
    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_mult_vect_term(sum_, sum_);

    pnl_vect_minus_vect(std_dev, sum_);
   
    pnl_vect_mult_scalar(std_dev, expon*expon);
    pnl_vect_mult_vect_term(vectSt_, vectSt_);
    pnl_vect_div_vect_term(std_dev, vectSt_);
    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_map_inplace(std_dev, sqrt);

}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *std_dev, PnlVect *dividende, PnlVect* deltachange, PnlVect* divRates, int country[], PnlMat* pastRates)
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
        mod_->assetDelta(path_, t, prodd_->T_, prodd_->nbTimeSteps_, rng_, past, dividende); // M simulations
        modRates_->assetDelta(changesPath_,t, prodd_->T_, prodd_->nbTimeSteps_, rng_,pastRates, divRates);

        for (int d=0; d< prodd_->size_; d++)
        {
            mod_->shiftAsset(shiftPath_, path_, d, fdStep_, t, TOverN);
            payoffPlus = prodd_->payoff(shiftPath_, changesPath_);
            mod_->shiftAsset(shiftPath_, path_, d, -fdStep_, t, TOverN);
            payoffMinus = prodd_->payoff(shiftPath_, changesPath_);
            resPayoff = payoffPlus - payoffMinus;

            LET(sum_, d) += resPayoff; // M addition
            LET(std_dev, d) += resPayoff * resPayoff;
        }
 
        for (int d=0; d< modRates_->size_; d++)
        {
            
            modRates_->shiftAsset(shiftpathchange_ , changesPath_ , d, fdStep_, t, TOverN);
            
            payoffPlus = prodd_->payoff(path_, shiftpathchange_);

            // cout << payoffPlus <<endl;
            mod_->shiftAsset(shiftpathchange_, changesPath_, d, -fdStep_, t, TOverN);
            payoffMinus = prodd_->payoff(path_, shiftpathchange_);
            // cout << payoffMinus <<endl;
            

            resPayoff = payoffPlus - payoffMinus;
            //sstd::cout << resPayoff<< std::endl;
            LET( ratesSum_ , d) += resPayoff; // M addition
            //LET(std_dev, d) += resPayoff * resPayoff;
        }
    }



    // ensemble de transformations pour obtenir le delta
    pnl_mat_get_row(vectSt_, past, past->m-1);
    pnl_vect_div_scalar(sum_, ((double)nbSamples_));
    pnl_vect_clone(delta, sum_);
    pnl_vect_mult_scalar(delta, expon);
    pnl_vect_div_vect_term(delta, vectSt_);

    //On fait pour les changes
    pnl_mat_get_row(vectStRate_ , pastRates, pastRates->m-1);
    pnl_vect_div_scalar(ratesSum_, ((double)nbSamples_));
    pnl_vect_clone(deltachange, ratesSum_);
    pnl_vect_mult_scalar(deltachange, expon);
    pnl_vect_div_vect_term(deltachange, vectStRate_);

    // for(int i = 0; i < mod_->size_; i++){
    //     if(country[i]!= 7)
    //     LET(deltachange, country[i]) = -GET(deltachange, country[i]) + GET(delta, i)/GET(modRates_->spot_, country[i])*GET(mod_->spot_, i);
    // }

    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_mult_vect_term(sum_, sum_);

    pnl_vect_minus_vect(std_dev, sum_);
   
    pnl_vect_mult_scalar(std_dev, expon*expon);
    pnl_vect_mult_vect_term(vectSt_, vectSt_);
    pnl_vect_div_vect_term(std_dev, vectSt_);
    pnl_vect_div_scalar(std_dev, (double)nbSamples_);
    pnl_vect_map_inplace(std_dev, sqrt);
}


void MonteCarlo::pAndL(int nbHedgeDate, double &errorHedge, PnlMat *marketData, double valLiqRef, PnlMat* pathRates, PnlVect* divStocks, PnlVect* divRates, int country[], PnlVect* vectexp)
{

    PnlVect *delta = pnl_vect_create(marketData->n);/*! vecteur contenant les deltas en t_i */
    PnlVect *deltaPrevious = pnl_vect_create(marketData->n); /*! vecteur contenant les deltas en t_(i-1) */

    PnlVect* deltaChange = pnl_vect_create(pathRates->n);
    PnlVect* deltapreviousChange = pnl_vect_create(pathRates->n);

    PnlVect *stdDevDelta = pnl_vect_create(marketData->n); /*! vecteur contenant les écart-types de delta */
    PnlMat *past = pnl_mat_create(prodd_->nbTimeSteps_ + 1, marketData->n); /*! trajectoires du passé */
    PnlMat* pastRates = pnl_mat_create( prodd_->nbTimeSteps_+1, pathRates->n );

    PnlMat *subPast = pnl_mat_create(1, marketData->n); /*! trajectoires du passé, de taille variable */
    PnlMat* subChangePath = pnl_mat_create(1, pathRates->n);
    FILE * f;
    f = fopen ("Pvalue.txt", "wt");
    if (f == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * fp;
    fp = fopen ("Payoff.txt", "wt");
    if (fp == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    FILE * Pdates;
    Pdates = fopen ("dates.txt", "wt");
    if (Pdates == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    double V = 0.;
    double prix = 0.;
    double std_dev = 0.;

    // creation dividende à 0 car sans dividendes
    //PnlVect *div = pnl_vect_create_from_zero(marketData->n);

    //int s = path_->n;
    MonteCarlo::delta(deltaPrevious, stdDevDelta, divStocks, deltapreviousChange, divRates, country );
    int HOverN = (int)(nbHedgeDate / prodd_->nbTimeSteps_);
    int TOverN = (int) prodd_->T_ * (marketData->m - 1) / prodd_->nbTimeSteps_;
    double TOverH = prodd_->T_/nbHedgeDate;
    double expon = exp(mod_->r_*TOverH);


    PnlVect vecLine = pnl_vect_wrap_mat_row(marketData, 0);
    PnlVect vectChangeLine = pnl_vect_wrap_mat_row(pathRates, 0);
    PnlVect* valuechange = pnl_vect_create(pathRates->n);
    pnl_vect_clone(valuechange, &vectChangeLine);
    pnl_vect_mult_vect_term(valuechange, vectexp);
    V = valLiqRef - pnl_vect_scalar_prod(deltaPrevious, &vecLine) - pnl_vect_scalar_prod(deltapreviousChange, valuechange);
    
    pnl_mat_set_row(pathRates, &vectChangeLine,0);
    pnl_mat_set_row(past, &vecLine, 0);

    int pastIndex = 1;

    pnl_mat_resize(subPast, pastIndex + 1, prodd_->size_);
    pnl_mat_resize(subChangePath, pastIndex+1, pathRates->n );
    // On a commenté car c'est la même chose que la ligne 215
    //vecLine = pnl_vect_wrap_mat_row(marketData, 0);
     
    pnl_mat_set_row(subPast,  &vecLine, 0);
    pnl_mat_set_row(subChangePath, &vectChangeLine,0);

    for (int tbrut = 1; tbrut < nbHedgeDate + 1; tbrut++) // chaque t est une date de rebalancement, c'est la grille fine
    {
        // Si on rebalance a chaque date données le marketData, alors tbrut = t;
        int t = (int) (tbrut*(marketData->m - 1) / nbHedgeDate);

        std::cout << "t = " << t << std::endl;
        
        // Attention Vigilance
        vecLine = pnl_vect_wrap_mat_row(marketData, t); // on recupere les données historique de la date t
        vectChangeLine = pnl_vect_wrap_mat_row(pathRates, t);
        if (t % TOverN == 0) // le t est un ti (une date de constatation)
        {
            pnl_mat_set_row(past, &vecLine, pastIndex);
            pnl_mat_set_row(pastRates, &vectChangeLine, pastIndex);
        }

        pnl_mat_set_row(subPast, &vecLine, pastIndex);
        pnl_mat_set_row(subChangePath, &vectChangeLine, pastIndex);
        MonteCarlo::delta(subPast, tbrut*TOverH, delta, stdDevDelta, divStocks,deltaChange, divRates, country, subChangePath); // on rebalance donc on calcule le delta
        pnl_vect_minus_vect(deltaPrevious, delta);
        pnl_vect_minus_vect(deltapreviousChange, deltaChange);
        pnl_vect_clone(valuechange, &vectChangeLine);
        pnl_vect_mult_vect_term(valuechange, vectexp);
        V = V * expon + pnl_vect_scalar_prod(deltaPrevious, &vecLine) + pnl_vect_scalar_prod( deltapreviousChange, valuechange);
        pnl_vect_clone(deltaPrevious, delta);
        pnl_vect_clone(deltapreviousChange, deltaChange);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // On compare ici la valeur du portefeuille à l'instant t avec le prix donné par le pricer
        // Ce seront les données à afficher dans notre application pour le portefeuille de couverture
       
        MonteCarlo::price(subPast, tbrut*TOverH, prix, std_dev, divStocks, divRates, subChangePath);
        double valeurPort = V + pnl_vect_scalar_prod(delta, &vecLine) + pnl_vect_scalar_prod(deltaChange, valuechange);

        std::cout << "En t = " << tbrut*TOverH << " V = " << valeurPort << " prix  = " << prix << endl;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        fprintf(f, "%lf \n", valeurPort);
        fprintf(fp, "%lf \n", prix);
        fprintf(Pdates, "%lf \n", tbrut *TOverH);
        if (t % TOverN == 0 && subPast->m < past->m) // le t est un ti, le nombre de ligne de subPast doit être strictement inférieur
        // a celui de past, sinon cela veut dire qu'on est à t=T;
        {
            // Problème ici, car lorsqu'on resize, toutes les données de subPast sont perdus
            pastIndex ++;
            pnl_mat_extract_subblock(subPast, past, 0, pastIndex + 1, 0, prodd_->size_); // on garde les données de cette date ti et on fait de la place pour les prochaines dates intermediaires t 
            pnl_mat_extract_subblock(subChangePath, pastRates, 0, pastIndex+1, 0, pathRates->n);
        }

    }
    fclose(f);
    fclose(fp);
    fclose(Pdates);

    errorHedge = V + pnl_vect_scalar_prod(delta, &vecLine) + pnl_vect_scalar_prod(deltaChange, valuechange) - prodd_->payoff(past); // calcule du PnL
    pnl_vect_free(&delta);
    pnl_vect_free(&deltaPrevious);
    pnl_vect_free(&stdDevDelta);
    pnl_mat_free(&past);
    pnl_mat_free(&subPast);
}
