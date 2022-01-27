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

    // Performance *perf = new Performance(observeDates, market, dates);
    Performance *perf = new Performance(Pdates, market, Pdates);
    perf->niveauInitial();

    // vector<string>  dates = Date::getListOfDates("2021-10-12", "2022-12-12");
    //PnlVect* volsim = pnl_vect_create_from_scalar(market->getNumOfActions(), 2);
    PnlVect* spots = perf->getNivInitAct();
    // pnl_mat_get_row(spots, path, 0);
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(),r,1,volatilities, spots, div);
    pnl_mat_chol(corrMat);
    bs->correlations_ = corrMat;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double fdstep = .01;
    int nbSample = 5000;

    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);
    double prix, std_dev;
    // mc->price(prix, std_dev);
    // PnlVect* delta = pnl_vect_create(market->getNumOfActions());
    // PnlVect* std_dev_delta = pnl_vect_create(market->getNumOfActions());
    // mc->delta(delta, std_dev_delta);
    // pnl_vect_print(delta);
    mc->price(prix, std_dev); 
    std::cout << "Prix en 0 " << prix <<std::endl;

    double prixt;

    //calcul du prix en t = auj

    vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");

    vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", "2021-12-15");

    PnlMat *pathFull = pnl_mat_new();
    market->getPathFromDates(pathFull, datesFrom2014ToToday);


    // Performance *perf = new Performance(observeDates, market, datesFrom2014To2022);
    //market->fillData(parser); // on a n'importe quoi dans le dictionnaire à cause du calcul du prix en 0 donc on reclean le dico
    //Performance *perfForPriceToday = new Performance(Pdates, market, Pdates);
    //MonteCarlo *mcForPriceToday = new MonteCarlo(bs, perfForPriceToday,fdstep,nbSample, rng);

    int nbDatesInPast = 15;
    PnlMat *past = pnl_mat_create(nbDatesInPast, market->getNumOfActions());
    // market->fiilPathMat(past, "2014-07-11", datesFrom2014ToToday.size());

    // perfForPriceToday->niveauInitial();

    pnl_mat_set_row(past, perf->getNivInitAct(), 0);

    PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
    for (int i = 0; i < nbDatesInPast - 1; i ++) {
        market->getSpotsFromDate(vecteurPast, Pdates[i]);
        pnl_mat_set_row(past, vecteurPast, i+1);
    }

    // pnl_mat_print(past);

    double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();
    mc->price(past, t, prixt, std_dev);

    cout << "prix auj " << prixt <<endl;


    //calcul P&L
    
    // int H = datesFrom2014To2022.size() - 1;
    int H = 17;
    double T = 1;

    pnl_mat_resize(path, 17, market->getNumOfActions());
    PnlVect* vectline = pnl_vect_new();
    for(int i = 0; i < past->m; i++){
        pnl_mat_get_row(vectline, past, i);
        pnl_mat_set_row(path, vectline ,i);
    }
    PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), r);
    bs->simul_market(past, T, rng, trend, H - 1, path);
    


    

    double errorHedge;

    mc->pAndL(H - 1, errorHedge, path, 1);

    SPDLOG_LOGGER_INFO(_logger, "ErrorHedge => {}", errorHedge);

    FILE * Pdatesfile;
    Pdatesfile = fopen ("dates.txt", "wt");
    if (Pdatesfile == NULL){
    std::cout << "Impossible d'ouvrir le fichier en écriture !" << std::endl;}

    for (int i = 0; i< Pdates.size(); i++){
        
        fprintf(Pdatesfile, "%s \n", Pdates[i].c_str());
    }

    pnl_vect_free(&volatilities);
    pnl_mat_free(&path); 
    pnl_mat_free(&pathFull);
    pnl_vect_free(&div);

    // on a pas le mme price en t si on calcule price en 0 avant ou pas 
    // le calcul de price en t ne marche pas si on a fait le calcul de price en 0 avant et qu'on reutilise les mm mc et perf
}