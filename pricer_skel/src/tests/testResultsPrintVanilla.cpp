#include <iostream>
#include <string>
#include "montecarlo/MonteCarlo.hpp"
#include "fs/ParseCsv.hpp"
#include "models/BlackScholesModel.hpp"
#include "financialProducts/VanillaCall.hpp"
#include "spdlog/log.hpp"
#include <cstdio>


int main(){
    double T = 1;
    int nbTimeSteps = 1;
    double K = 100;
    int size = 1;
    double r = 0.04879;
    double rho = 1;
    int nbSamples = 50000;
    double fdStep = 0.01;
    int nbHedgeDate = 365;
    double errorHedge;

    // For test starting in date t
    double t = 0.5;

    // data for the theorical price
    double  Nd1 = .63455805571885;
    double Nd2 = .557230023739951; 

    // For test starting in date t
    double  Nd1t = .596078398587738;
    double Nd2t = .540537533000572; 

    // variables used to test the 95% proportion
    int compteurJuste = 0;
    int compteurGlobal = 5000;
    bool juste;
    double proportion;
    double precision = 0.01;
    double precisionPnL = 0.2;
    FILE * f;
    f = fopen ("prix0.txt", "wt");
    if (f == NULL)
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;

    log::init();
        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
        PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

        Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
        BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
        MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

        double theoricalPrice  = 100. * Nd1 - K*exp(-r*T)*Nd2;

        double price;
        double std_dev;

        for (int i = 0; i < compteurGlobal; i ++) {
            mc->price(price, std_dev);
            fprintf(f, "%lf \n", price - theoricalPrice);
            juste = abs(price - theoricalPrice) < 1.96*std_dev;
            if (juste){
                compteurJuste ++;
            }
        }

        proportion = (double) compteurJuste / compteurGlobal;

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_rng_free(&rng);
        delete mc;
        delete bs;
        delete vanille;
}