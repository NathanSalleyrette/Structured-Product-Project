#include <iostream>
#include <string>
#include "montecarlo/MonteCarlo.hpp"
#include "fs/ParseCsv.hpp"
#include "models/BlackScholesModel.hpp"
#include "financialProducts/VanillaCall.hpp"
using namespace std;

int main(int argc, char **argv){
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

    double  Nd1 = .63455805571885;
    double Nd2 = .557230023739951; 
    double theoricalPrice  = 100. * Nd1 - K*exp(-r*T)*Nd2;

    
    double price;
    double std_dev;
    ProduitDerive *vanille = new VanillaCall(T, nbTimeSteps, K);
    BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);

    MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);
    mc->price(price, std_dev);
    bool juste = abs(price - theoricalPrice) < 1.96*std_dev;
    if (juste){
        std::cout<<"Le prix est valide"<<endl;
    }else{
        std::cout<<"Le prix n'est pas valide"<<endl;
    }

//On passe maintenant au calcul du delta théorique

    PnlVect *std_dev_delta = pnl_vect_create_from_scalar(1,1);
    PnlVect *delta = pnl_vect_create_from_scalar(1,1);
    mc->delta(delta, std_dev_delta);
    juste = abs(GET(delta,0) - Nd1) < 1.96*GET(std_dev_delta,0);
    if (juste){
        std::cout<<"Le delta est valide"<<endl;
    }else{
        std::cout<<"Le delta n'est pas valide"<<endl;
    }

    pnl_vect_free(&std_dev_delta);
    pnl_vect_free(&delta);
    delete mc;
    delete bs;
    

}

