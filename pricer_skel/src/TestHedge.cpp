#include <iostream>
#include <string>
#include "MonteCarlo.hpp"
#include "ParseCsv.hpp"
#include "BlackScholesModel.hpp"
#include "VanillaCall.hpp"

using namespace std;

int main(int argc, char **argv)
{
<<<<<<< HEAD
    // double T, r, strike, rho;
    // PnlVect *spot, *sigma, *divid, *trend;
    // string type;
    // int size, N, n_hedging_dates;
    // size_t n_samples;

    // char *infile = argv[2];
    // Param *P = new Parser(infile);

    // P->extract("option type", type);
    // P->extract("maturity", T);
    // P->extract("option size", size);
    // P->extract("spot", spot, size);
    // P->extract("volatility", sigma, size);
    // P->extract("interest rate", r);
    // if (P->extract("payoff coefficients", divid, size, true) == false)
    // {
    //     divid = pnl_vect_create_from_zero(size);
    // }
    // if (P->extract("trend", trend, size, true) == false)
    // {
    //     trend = pnl_vect_create_from_zero(size);
    // }
    // if (P->extract("strike", strike, true) == false)
    // {
    //     strike = 0;
    // }
    // P->extract("sample number", n_samples);
    // P->extract("hedging dates number", n_hedging_dates);
    // P->extract("timestep number", N);
    // P->extract("correlation", rho);

    // Option *option;
    // if (type == "basket") {
    //     option = new BasketCall(T, size, N, strike, divid);
    // }
    // else if (type == "asian"){
    //     option = new AsianOption(T, size, N, strike, divid);
    // }
    // else if (type == "performance"){
    //     option = new PerfOption(T, size, N, divid);
    // }
    // else{
    //     cout << "ERREUR DE TYPE" << endl;
    // }
    
    // BlackScholesModel *bs = new BlackScholesModel(option->size_, r, rho, sigma, spot, trend);

    // double std_dev;
    // double prix;

    // PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    // pnl_rng_sseed(rng, time(NULL));

    // MonteCarlo *mc = new MonteCarlo(bs, option, 0.01, n_samples, rng);

    // // PARTIE CREATION DU MARKET DATA
    // // on recupere la matrice des donnees
    // char *data = argv[1];
    // PnlMat *marketData = pnl_mat_create_from_file(data);

    // mc->price(prix, std_dev);

    // double errorHedge;
    // mc->pAndL(n_hedging_dates, errorHedge, marketData);

    // HedgingResults res(prix, std_dev, errorHedge);
    // std::cout << res << std::endl;

    // pnl_vect_free(&spot);
    // pnl_vect_free(&sigma);
    // pnl_vect_free(&divid);
    // pnl_mat_free(&marketData);
    // pnl_rng_free(&rng);
    // pnl_vect_free(&trend);
    // delete P;
    // delete mc;
    // delete option;
    // delete bs;

    double T = 1;
    int nbTimeSteps = 3;
    double K = 100;
    int size = 1;
    double r = 0.05;
    double rho = 1;
    int nbSamples = 10000;
    double fdStep = 0.01;
    int nbHedgeDate = 12;
=======
    double T = 1;
    int nbTimeSteps = 1;
    double K = 100;
    int size = 1;
    double r = 0.04879;
    double rho = 1;
    int nbSamples = 50000;
    double fdStep = 0.01;
    int nbHedgeDate = 365;
>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    double errorHedge;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

<<<<<<< HEAD
    PnlVect *sigma = pnl_vect_create_from_scalar(1, 1.9692);
    PnlVect *spot = pnl_vect_create_from_scalar(1, 100);
    ParseCsv *parser = new ParseCsv("../data/data.csv", 13);

    
    Option *vanille = new VanillaCall(T, nbTimeSteps, K);
    BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);

    MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);
    mc->pAndL(nbHedgeDate, errorHedge, parser->marketData_);
    cout << errorHedge << endl;
    // pnl_mat_print(parser->marketData_);
=======
    PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

// --------------------------------
    // N = 3, H = 12

    // ParseCsv *parser = new ParseCsv("../data/data.csv", 13, 1);

    // ProduitDerive *vanille = new VanillaCall(T, nbTimeSteps, K);
    // BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);

    // MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);
    // mc->pAndL(nbHedgeDate, errorHedge, parser->marketData_);
    // cout << errorHedge << endl;
    // pnl_mat_print(parser->marketData_);

    // delete parser;
    // delete vanille;
    // delete bs;
    // delete mc;

//    ----------------------------------------------
    // Test parseur fichier avec plusieurs colonnes

    // ParseCsv *parser2 = new ParseCsv("../data/dataBasket.csv", 366, 5);
    // pnl_mat_print(parser2->marketData_);
    // delete parser2;

// --------------------------------------
    // N = 1, H = 365


    ParseCsv *parser3 = new ParseCsv("../data/callVanilleProf.csv", 366, 1);
    
    ProduitDerive *vanille = new VanillaCall(T, nbTimeSteps, K);
    BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);

    MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);
    mc->pAndL(nbHedgeDate, errorHedge, parser3->marketData_);
    cout << errorHedge << endl;
    pnl_mat_print(parser3->marketData_);

    delete parser3;
    delete vanille;
    delete bs;
    delete mc;
    

    // --------------------------


    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_rng_free(&rng);

>>>>>>> e0d2490cd837cd8999735169ccb4ec68976b95c5
    exit(0);
}
