#include <string>
#include "utils/MarketData.hpp"
#include "fs/ParseYahooCsv.hpp"
#include "utils/Utils.hpp"
#include "utils/Date.hpp"
#include "models/BlackScholesModel.hpp"
#include "montecarlo/MonteCarlo.hpp"
#include "financialProducts/Performance.hpp"
#include "spdlog/log.hpp"

#include <map>

using namespace std;

int main(int argc, char **argv)
{

    log::init();
    double r = 0.04879;
    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser); // on remplit le dictionnaire avec les données du csv

    // calcul de la volatilité et la correlation
    PnlMat* pathForVol = pnl_mat_create(1,1);
    market->fillPathMat(pathForVol, "2021-12-10", 5);

    PnlVect* volatilities = pnl_vect_create(market->getNumOfActions());
    Utils::volsOnMat(volatilities, pathForVol);
    
    // On divise par deux car on prend 2 dates dans l'année dans ce test
    pnl_vect_mult_scalar(volatilities, 1./2.);

    PnlMat* corrMat = pnl_mat_create(market->getNumOfActions(), market->getNumOfActions());
    Utils::correlationMatrix(pathForVol, corrMat);
    // pnl_mat_mult_double(corrMat, 1./2.);

    // creation de performance


    vector<string> Pdates = 
     { "2015-01-12", "2015-07-13"
     , "2016-01-11", "2016-07-11"
     , "2017-01-11", "2017-07-11"
     , "2018-01-11", "2018-07-11"
     , "2019-01-11", "2019-07-11"
     , "2020-01-11", "2020-07-11"
     , "2021-01-11", "2021-07-11"
     , "2022-01-11", "2022-07-11"};

    // Performance *perf = new Performance(observeDates, market, dates);
    Performance *perf = new Performance(Pdates, market, Pdates);
    perf->niveauInitial();

    // vector<string>  dates = Date::getListOfDates("2021-10-12", "2022-12-12");
    //PnlVect* volsim = pnl_vect_create_from_scalar(market->getNumOfActions(), 2);
    PnlVect* spots = perf->getNivInitAct();
    // pnl_mat_get_row(spots, path, 0);
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(),r,1,volatilities, spots);
    pnl_mat_chol(corrMat);
    bs->correlations_ = corrMat;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double fdstep = .01;
    int nbSample = 2000;

    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);

    // on calcul t

    vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");

    vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", "2021-12-15");

    double t = (double)(datesFrom2014ToToday.size())/datesFrom2014To2022.size();

    double ti = (double)15/17; // t0 = 14 t1 = 1-15 t2 = 7-15 t14 = 7-21 = 15/17

    // 14/16 = 0.875
    // 15/17 = 0.882

    // 15/16 = 0.9375 > t
    // 16/17 = 0.94


    // Performance *perf = new Performance(observeDates, market, datesFrom2014To2022);
    // market->fillData(parser); // on a n'importe quoi dans le dictionnaire à cause du calcul du prix en 0 donc on reclean le dico
    // Performance *perf = new Performance(Pdates, market, Pdates);
    // MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);

    // on construit la matrice past
    int nbDatesInPast = 15;
    PnlMat *past = pnl_mat_create(nbDatesInPast, market->getNumOfActions());
    // market->fiilPathMat(past, "2014-07-11", datesFrom2014ToToday.size());

    // perf->niveauInitial();

    pnl_mat_set_row(past, perf->getNivInitAct(), 0);

    PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
    for (int i = 0; i < nbDatesInPast - 1; i ++) {
        market->getSpotsFromDate(vecteurPast, Pdates[i]);
        pnl_mat_set_row(past, vecteurPast, i+1);
    }

    // pnl_mat_print(past);

    double prix, std_dev;


    mc->price(past, t, prix, std_dev);

    cout << "prix auj " << prix <<endl;


    // //calcul P&L
    
    // int H = datesFrom2014To2022.size() - 1;
    // double T = 1;
    // market->fiilPathMat(past, "2014-07-11", datesFrom2014ToToday.size());
    // market->fiilPathMat(path, "2014-07-11", datesFrom2014To2022.size());
    // PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), r);
    // bs->simul_market(past, T, rng, trend, H, path);

    

    // double errorHedge;

    // mc->pAndL(H, errorHedge, path, 1);

    // cout << errorHedge << endl;


    pnl_vect_free(&volatilities);
    pnl_mat_free(&pathForVol);
}