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
#include<cstdio>

using namespace std;

int main(int argc, char **argv)
{
    log::init();
    std::shared_ptr<spdlog::logger> _logger = spdlog::get("MainLogs");
    double r = 0.04879;
    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser); // on remplit le dictionnaire avec les données du csv

    // calcul de la volatilité et la correlation
    PnlMat* path = pnl_mat_create(1,1);
    market->fillPathMat(path, "2021-12-10", 5);

    PnlVect* volatilities = pnl_vect_create(market->getNumOfActions());
    Utils::volsOnMat(volatilities, path);
    pnl_vect_mult_scalar(volatilities, 1. / 16.);

    PnlMat* corrMat = pnl_mat_create(market->getNumOfActions(), market->getNumOfActions());
    Utils::correlationMatrix(path, corrMat);
    pnl_mat_mult_double(corrMat, 1.);

    // creation dividende à 0 car sans dividendes
    PnlVect *div = pnl_vect_create_from_zero(market->getNumOfActions());

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

    Performance *perf = new Performance(Pdates, market, Pdates);
    perf->niveauInitial();

    PnlVect* spots = perf->getNivInitAct();
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(),r,1,volatilities, spots, div);
    pnl_mat_chol(corrMat);
    bs->correlations_ = corrMat;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double fdstep = .01;
    int nbSample = 5000;

    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);
    double prix, std_dev;
    mc->price(prix, std_dev); 
    std::cout << "Prix en 0 " << prix <<std::endl;

    // construction de la matrice contenant toutes les données historiques

    vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");

    vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", "2021-12-15");

    PnlMat *pathFull = pnl_mat_new();
    market->getPathFromDates(pathFull, datesFrom2014ToToday);


    //calcul P&L

    int H = datesFrom2014To2022.size();
    double T = 1;

    PnlMat *past = pnl_mat_create_from_zero(H, market->getNumOfActions());

    PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), r);
    bs->simul_market(pathFull, T, rng, trend, H - 1, past);
    

    double errorHedge;

    mc->pAndL(H - 1, errorHedge, past, 1);

    SPDLOG_LOGGER_INFO(_logger, "ErrorHedge => {}", errorHedge);

    pnl_vect_free(&volatilities);
    pnl_mat_free(&path); 
    pnl_mat_free(&pathFull);
    pnl_mat_free(&past);
    pnl_vect_free(&div);

}