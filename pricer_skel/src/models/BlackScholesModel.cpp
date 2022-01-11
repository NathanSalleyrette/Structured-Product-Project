#include <iostream>
#include "models/BlackScholesModel.hpp"
#include <cmath>
#include <cassert>
#include "spdlog/log.hpp"

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot)
{
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = sigma;
    spot_ = spot;
    PnlMat *correlations = pnl_mat_create(size, size);
    pnl_mat_set_all(correlations, rho);
    pnl_mat_set_diag(correlations, 1, 0);
    pnl_mat_chol(correlations);
    correlations_ = correlations;
    G_ = pnl_vect_create_from_scalar(size_, 0);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_mat_free(&correlations_);
    pnl_vect_free(&G_);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
    double interval = T / nbTimeSteps;
    double sqrtInterval = sqrt(interval);
    double sigma;
    PnlVect vecLine;


    pnl_mat_set_row(path, spot_, 0); // la première ligne de path contient les spot en t=0
    for(int i=1; i<=nbTimeSteps; i++){
        pnl_vect_rng_normal(G_, size_, rng);
        for (int d=0;d<size_;d++) {
            //std::cout << d<<std::endl;
            sigma = GET(sigma_, d);
            // std::cout<<sigma<<std::endl;
            vecLine = pnl_vect_wrap_mat_row(correlations_, d);
            MLET(path, i, d) = MGET(path, i-1, d) * exp((r_ - (sigma * sigma)/2) * interval + sigma * sqrtInterval * pnl_vect_scalar_prod(G_, &vecLine));
        }
    }
}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past)
{
    double timeDelta = T / nbTimeSteps;
    double sqrtTimeDelta = sqrt(timeDelta);
    double interval;
    double sigma;
    PnlVect vecLine;
    

    /// On recopie tous les elements de past dans path
    for (int i=0; i<past->m; i++){
        vecLine = pnl_vect_wrap_mat_row(past, i);
        pnl_mat_set_row(path, &vecLine, i);
    }

    int simulationStart = past->m - 1;
    // std::cout << MGET(path, simulationStart, 0) << std::endl;
    // std::cout << simulationStart << std::endl;
    interval = simulationStart * timeDelta - t;
    double sqrtInterval = sqrt(interval);
    // disjonction du cas si t est trop loin de t_{i+1}, on doit modifier la dernière valeur
    if(abs(interval) > 1.e-4){
        // std::cout << interval << std::endl;
        // std::cout << "on rentre là dedans" << std::endl;

        //simulation de ti+1
        pnl_vect_rng_normal(G_, size_, rng);
        for(int d = 0; d<size_; d++){
            sigma = GET(sigma_, d);
            vecLine = pnl_vect_wrap_mat_row(correlations_, d);
            MLET(path, simulationStart, d) *= exp((r_ - (sigma * sigma)/2) * interval + sigma * sqrtInterval * pnl_vect_scalar_prod(G_, &vecLine));
        }
    }
    //simulation de ti+2..., tN

    // std::cout << simulationStart << std::endl;
    // std::cout << MGET(path, simulationStart, 0) << std::endl;
    // assert(1==2);
    for(int i=simulationStart + 1; i<=nbTimeSteps; i++){
        pnl_vect_rng_normal(G_, size_, rng);
        for (int d = 0; d<size_; d++) {
            sigma = GET(sigma_, d);
            vecLine = pnl_vect_wrap_mat_row(correlations_, d);
            MLET(path, i, d) = MGET(path, i-1, d) * exp((r_ - (sigma * sigma)/2) * timeDelta + sigma * sqrtTimeDelta * pnl_vect_scalar_prod(G_, &vecLine));
        }
    }
}

void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) // timestep = T/N 
{
    double nbTimeSteps = path->m-1;
    int simulation_start;

    int i = (int)(t/timestep);
    double ti = i * timestep;


    //on vérifie si l'on est sur une date de constatation (donc une date % T/N)
    if (t-ti <= 10E-10)
    {
        simulation_start = i;
    }
    else
    {
        simulation_start = i+1;
    }
    pnl_mat_clone(shift_path, path);

    for (int i = simulation_start; i <= nbTimeSteps; i++){
        MLET(shift_path, i, d) *= (1+h);
    }
}

void BlackScholesModel::simul_market(PnlMat *past, double T, PnlRng *rng, PnlVect *trend, int nbHedgeDate, PnlMat *path)
{   
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    SPDLOG_LOGGER_INFO(_logger, "Starting siluating path");
    int start = past->m;
    double interval = T / nbHedgeDate;
    double sqrtInterval = sqrt(interval);
    double mu;
    double sigma;
    PnlVect vecLine;

    pnl_mat_resize(path, nbHedgeDate+1, size_);
    pnl_mat_set_row(path, spot_, 0);

    //on rempli path avec les données du passé qui sont dans past
    for (int i=1; i<start; i++){
        for (int j=0; j<size_; j++){
            MLET(path, i, j) = MGET(past, i, j);
        }
    }
    if (start == 0) start=1; // si past est vide

    // simule la suite des données jusqu'à la maturité
    for(int i=start; i <= nbHedgeDate; i++){
        pnl_vect_rng_normal(G_, size_, rng);
        for (int d=0; d<size_;d++) {
            sigma = GET(sigma_, d);
            mu = GET(trend, d);
            vecLine = pnl_vect_wrap_mat_row(correlations_, d);
            MLET(path, i, d) = MGET(path, i-1, d) * exp((mu - (sigma * sigma)/2) * interval + sigma * sqrtInterval * pnl_vect_scalar_prod(G_, &vecLine));
        }
    }
    SPDLOG_LOGGER_INFO(_logger, "Path simulated");
}
