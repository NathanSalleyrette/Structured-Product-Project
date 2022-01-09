#include <string>
#include "../../headers/MarketData.hpp"
#include "../../headers/fs/ParseYahooCsv.hpp"
#include "../../headers/utils/Utils.hpp"
#include "../../headers/utils/Date.hpp"
#include "../../headers/models/BlackScholesModel.hpp"
#include "../../headers/montecarlo/MonteCarlo.hpp"
#include "../../headers/financialProducts/Performance.hpp"
#include <map>

using namespace std;

int main(int argc, char **argv)
{

    double r = 0.04879;
    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser);

    PnlMat* path = pnl_mat_create(1,1);
    market->fiilPathMat(path, "2021-12-10", 5);

    PnlVect* volatilities = pnl_vect_create(market->getNumOfActions());
    Utils::volsOnMat(volatilities, path);
    pnl_vect_mult_scalar(volatilities, 1./365.);

    PnlMat* corrMat = pnl_mat_create(market->getNumOfActions(), market->getNumOfActions());
    Utils::correlationMatrix(path, corrMat);
    pnl_mat_mult_double(corrMat, 1./365.);

    vector<string>  dates = Date::getListOfDates("2021-10-12", "2022-12-12");
    //PnlVect* volsim = pnl_vect_create_from_scalar(market->getNumOfActions(), 2);
    PnlVect* spots = pnl_vect_create(path->n);
    pnl_mat_get_row(spots, path, 0);
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(),r,1,volatilities, spots);
    pnl_mat_chol(corrMat);
    bs->correlations_ = corrMat;

    vector<string> Pdates = 
     { "2015-01-12", "2015-07-13"
     , "2016-01-11", "2016-07-11"
     , "2017-01-11", "2017-07-11"
     , "2018-01-11", "2018-07-11"
     , "2019-01-11", "2019-07-11"
     , "2020-01-11", "2020-07-11"
     , "2021-01-11", "2021-07-11"
     , "2022-01-11", "2022-07-11"};

    vector<string> observeDates = {};

    Performance *perf = new Performance(observeDates, market, dates);

    for (int i = 0; i < Pdates.size(); i++) {
        observeDates.push_back(Pdates[i]);
        perf->setObservationDates(observeDates);
    }

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double fdstep = .01;
    int nbSample = 2;

    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);
    double prix, std_dev;
    // mc->price(prix, std_dev);
    PnlVect* delta = pnl_vect_create(market->getNbStocks());
    PnlVect* std_dev_delta = pnl_vect_create(market->getNbStocks());
    //mc->delta(delta, std_dev_delta);
    mc->price(prix, std_dev);
    std::cout << "Prix" << prix <<std::endl;

    //calcul du prix en t = auj

    vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");

    vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", "2021-12-15");

    PnlMat *past = pnl_mat_create(1,1);
    market->fiilPathMat(past, "2014-07-11", datesFrom2014ToToday.size());

    Performance *perfForPriceToday = new Performance(observeDates, market, datesFrom2014To2022);
    MonteCarlo *mcForPriceToday = new MonteCarlo(bs, perfForPriceToday,fdstep,nbSample, rng);

    double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();
    mcForPriceToday->price(past, t, prix, std_dev);



    cout << "prix auj " << prix <<endl;


    //calcul P&L
    int H = datesFrom2014To2022.size() - 1;
    double T = 1;
    PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), r);
    bs->simul_market(past, T, rng, trend, H, path);

    double errorHedge;

    mc->pAndL(H, errorHedge, path, 1);

    cout << errorHedge << endl;


    pnl_vect_free(&volatilities);
    pnl_mat_free(&path); 
}