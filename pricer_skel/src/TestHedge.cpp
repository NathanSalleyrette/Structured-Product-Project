#include <iostream>
#include <string>
#include "MonteCarlo.hpp"
#include "ParseCsv.hpp"
#include "BlackScholesModel.hpp"
#include "VanillaCall.hpp"

using namespace std;

int main(int argc, char **argv)
{
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

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

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

    exit(0);
}
