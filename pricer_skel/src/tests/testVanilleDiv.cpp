#include <iostream>
#include <string>
#include "montecarlo/MonteCarlo.hpp"
#include "fs/ParseCsv.hpp"
#include "models/BlackScholesModel.hpp"
#include "financialProducts/VanillaCall.hpp"
#include "gtest/gtest.h"
#include "spdlog/log.hpp"

namespace
{
    // Vanilla Call's information
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
    double  Nd1 = .537425562402588;
; 

    // For test starting in date t
    double  Nd1t = .526483324441164;

    // variables used to test the 95% proportion
    int compteurJuste = 0;
    int compteurGlobal = 5000;
    // int compteurGlobal = 5;
    bool juste;
    double proportion;
    double precision = 0.01;
    double precisionPnL = 0.2;

    TEST(Vanille, Delta)
    {
        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
        PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

        // creation dividende à 0 car sans dividendes
        PnlVect *div = pnl_vect_create_from_scalar(1, 0.05);
        
        Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
        BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
        MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

        PnlVect *std_dev_delta = pnl_vect_create_from_scalar(1,1);
        PnlVect *delta = pnl_vect_create_from_scalar(1,1);


        compteurJuste = 0;

        double actu = exp(-GET(div,0)*T);
        double prix, std_dev;

        for (int i = 0; i < compteurGlobal; i ++) {
            
            mc->delta(delta, std_dev_delta, div);
            juste = abs(GET(delta,0) - actu*Nd1) < 1.96*GET(std_dev_delta,0);
            if (juste){
                compteurJuste ++;
            }
        }

        proportion = (double) compteurJuste / compteurGlobal;

        EXPECT_NEAR(0.95, proportion, precision);

        pnl_vect_free(&std_dev_delta);
        pnl_vect_free(&delta);

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_rng_free(&rng);
        pnl_vect_free(&div);
        delete mc;
        delete bs;
        delete vanille;
    }

    TEST(Vanille, DeltaEnt)
    {

        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
        PnlVect *spot = pnl_vect_create_from_scalar(1, 100);
        PnlMat *past = pnl_mat_create_from_scalar(2,1, 100);

        // creation dividende à 0 car sans dividendes
        PnlVect *div = pnl_vect_create_from_scalar(1, 0.05);

        Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
        BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
        MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

        PnlVect *std_dev_delta = pnl_vect_create_from_scalar(1,1);
        PnlVect *delta = pnl_vect_create_from_scalar(1,1);

        compteurJuste = 0;

        double actu = exp(-GET(div,0)*(T-t));

        // std::cout << "ref : " << actu*Nd1t << std::endl;

        for (int i = 0; i < compteurGlobal; i ++) {
            mc->delta(past, t, delta, std_dev_delta, div);
            juste = abs(GET(delta,0) - actu*Nd1t) < 1.96*GET(std_dev_delta,0);
            // pnl_vect_print(delta);
            if (juste){
                compteurJuste ++;
            }
        }

        proportion = (double) compteurJuste / compteurGlobal;

        EXPECT_NEAR(0.95, proportion, precision);

        pnl_vect_free(&std_dev_delta);
        pnl_vect_free(&delta);

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_mat_free(&past);
        pnl_rng_free(&rng);
        pnl_vect_free(&div);
        delete mc;
        delete bs;
        delete vanille;
    }
}
