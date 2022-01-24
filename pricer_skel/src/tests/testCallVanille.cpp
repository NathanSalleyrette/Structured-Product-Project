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


    TEST(Vanille, PnL)
    {
        log::init();

        nbHedgeDate = 365;
        nbTimeSteps = 1;

        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
        PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

        Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
        BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
        MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

        ParseCsv *parser = new ParseCsv("../data/callVanilleProf.csv", 366, 1);

        double prix, stddev;

        mc->price(prix, stddev);
        mc->pAndL(nbHedgeDate, errorHedge, parser->marketData_, prix);
        cout << "Nombre de rebalancement : " << nbHedgeDate << endl; 
        cout << "Erreur : " << errorHedge << endl;

        EXPECT_NEAR(0, errorHedge, precisionPnL);

        delete parser;
        delete vanille;
        delete bs;
        delete mc;

        pnl_vect_free(&spot);
        pnl_vect_free(&sigma);
        pnl_rng_free(&rng);
    }

    // TEST(Vanille, Prix)
    // {
    //     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //     pnl_rng_sseed(rng, time(NULL));

    //     PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    //     PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

    //     Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
    //     BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    //     MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

    //     double theoricalPrice  = 100. * Nd1 - K*exp(-r*T)*Nd2;

    //     double price;
    //     double std_dev;

    //     for (int i = 0; i < compteurGlobal; i ++) {
    //         mc->price(price, std_dev);
    //         juste = abs(price - theoricalPrice) < 1.96*std_dev;
    //         if (juste){
    //             compteurJuste ++;
    //         }
    //     }

    //     proportion = (double) compteurJuste / compteurGlobal;

    //     EXPECT_NEAR(0.95, proportion, precision);

    //     pnl_vect_free(&spot);
    //     pnl_vect_free(&sigma);
    //     pnl_rng_free(&rng);
    //     delete mc;
    //     delete bs;
    //     delete vanille;

    // }

    // TEST(Vanille, Delta)
    // {
    //     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //     pnl_rng_sseed(rng, time(NULL));

    //     PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    //     PnlVect *spot = pnl_vect_create_from_scalar(1, 100);
        
    //     Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
    //     BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    //     MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

    //     PnlVect *std_dev_delta = pnl_vect_create_from_scalar(1,1);
    //     PnlVect *delta = pnl_vect_create_from_scalar(1,1);


    //     compteurJuste = 0;

    //     for (int i = 0; i < compteurGlobal; i ++) {
    //         mc->delta(delta, std_dev_delta);
    //         juste = abs(GET(delta,0) - Nd1) < 1.96*GET(std_dev_delta,0);
    //         if (juste){
    //             compteurJuste ++;
    //         }
    //     }

    //     proportion = (double) compteurJuste / compteurGlobal;

    //     EXPECT_NEAR(0.95, proportion, precision);

    //     pnl_vect_free(&std_dev_delta);
    //     pnl_vect_free(&delta);

    //     pnl_vect_free(&spot);
    //     pnl_vect_free(&sigma);
    //     pnl_rng_free(&rng);
    //     delete mc;
    //     delete bs;
    //     delete vanille;
    // }

    // TEST(Vanille, PrixEnt)
    // {
    //     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //     pnl_rng_sseed(rng, time(NULL));

    //     PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    //     PnlVect *spot = pnl_vect_create_from_scalar(1, 100);
    //     PnlMat *past = pnl_mat_create_from_scalar(2,1, 100);

    //     double theoricalPrice  = 100. * Nd1t - K*exp(-r*(T -t))*Nd2t;
    //     double price;
    //     double std_dev;

    //     Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
    //     BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    //     MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

    //     compteurJuste = 0;

    //     for (int i = 0; i < compteurGlobal; i ++) {
    //         mc->price(past, t, price, std_dev);
    //         juste = abs(price - theoricalPrice) < 1.96*std_dev;
    //         if (juste){
    //             compteurJuste ++;
    //         }
    //     }

    //     proportion = (double) compteurJuste / compteurGlobal;

    //     EXPECT_NEAR(0.95, proportion, precision);

    //     pnl_vect_free(&spot);
    //     pnl_vect_free(&sigma);
    //     pnl_mat_free(&past);
    //     pnl_rng_free(&rng);
    //     delete mc;
    //     delete bs;
    //     delete vanille;

    // }

    // TEST(Vanille, DeltaEnt)
    // {

    //     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //     pnl_rng_sseed(rng, time(NULL));

    //     PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    //     PnlVect *spot = pnl_vect_create_from_scalar(1, 100);
    //     PnlMat *past = pnl_mat_create_from_scalar(2,1, 100);

    //     Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
    //     BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    //     MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

    //     PnlVect *std_dev_delta = pnl_vect_create_from_scalar(1,1);
    //     PnlVect *delta = pnl_vect_create_from_scalar(1,1);

    //     compteurJuste = 0;

    //     for (int i = 0; i < compteurGlobal; i ++) {
    //         mc->delta(past, t, delta, std_dev_delta);
    //         juste = abs(GET(delta,0) - Nd1t) < 1.96*GET(std_dev_delta,0);
    //         if (juste){
    //             compteurJuste ++;
    //         }
    //     }

    //     proportion = (double) compteurJuste / compteurGlobal;

    //     EXPECT_NEAR(0.95, proportion, precision);

    //     pnl_vect_free(&std_dev_delta);
    //     pnl_vect_free(&delta);

    //     pnl_vect_free(&spot);
    //     pnl_vect_free(&sigma);
    //     pnl_mat_free(&past);
    //     pnl_rng_free(&rng);
    //     delete mc;
    //     delete bs;
    //     delete vanille;
    // }



    // TEST(Vanille, PnLWithPricePlusMargin)
    // {

    //     PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    //     pnl_rng_sseed(rng, time(NULL));

    //     PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    //     PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

    //     Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
    //     BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    //     MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

    //     ParseCsv *parser = new ParseCsv("../data/callVanilleProf.csv", nbHedgeDate + 1, 1);

    //     double prix, stddev;
    //     double margin = 1;

    //     mc->price(prix, stddev);
    //     mc->pAndL(nbHedgeDate, errorHedge, parser->marketData_, prix + margin);

    //     EXPECT_NE(0, errorHedge);

    //     delete parser;
    //     delete vanille;
    //     delete bs;
    //     delete mc;


    //     pnl_vect_free(&spot);
    //     pnl_vect_free(&sigma);
    //     pnl_rng_free(&rng);
    // }
}
