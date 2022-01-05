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
    int size = 1;
    double rho = 1;
    double r = 0.04879;
    int nbHedgeDate = 19;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
    PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

    double prix, stddev;

    ParseCsv *parser = new ParseCsv("../data/data.csv", 13, 1);
    PnlMat *past = pnl_mat_create(0, 0);
    
    BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
    PnlMat *path = pnl_mat_create_from_zero(nbHedgeDate+1, size);
    PnlVect *trend = pnl_vect_create_from_scalar(size, r);

    cout << "Avec passé :" << endl;
    bs->simul_market(parser->marketData_, T, rng, trend, nbHedgeDate, path);
    pnl_mat_print(path);

    cout << "\nSans passé :" << endl;
    bs->simul_market(past, T, rng, trend, nbHedgeDate, path);
    pnl_mat_print(path);

    delete parser;
    delete bs;


    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_rng_free(&rng);
    pnl_mat_free(&path);
    pnl_vect_free(&trend);

    exit(0);  
}