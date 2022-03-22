#include <string>
#include "utils/MarketData.hpp"
#include "fs/ParseYahooCsv.hpp"
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
    string pathFiles = "../data/RATE";
    string pathData = "../data/DATA";
    
    int initialWindow = 10;
    string refDate = "2021-12-10"; // Date vers laquelle on veut créer notre fenêtre
    string endFinishDate = "2021-12-15";
    double fdstep = .01; // 1+h pour les deltas

    int nbTimeSteps= 17;

    double T = 1;
    int nbSample = 5000;
    
    map<string, double> rPerCountry = { {"EUR", 0./100.}, {"USD", 0.5/100.}, {"JAP", -0.1/100.}, {"GBP", 0.75/100}, {"CHF", -0.75/100}, {"BRZ", 11.75/100} , {"CAD", 0.5/100}, {"MXN", 6/100}};
   
   // Facteur multiplié à la vol annuelle 
    double divVol = sqrt(8. * 252./(nbTimeSteps-1));
    
    vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", endFinishDate);
    vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");


    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser, pathData); // on remplit le dictionnaire avec les données du csv
    MarketData *rates = new MarketData();
    
    rates->fillData(parser, pathFiles);

    // calcul de la volatilité et la correlation
    PnlMat* path = pnl_mat_create(1,1);
    market->fillPathMat(path, refDate, initialWindow);

    PnlVect* volatilities = pnl_vect_create(market->getNumOfActions());
    Utils::volsOnMat(volatilities, path);
    pnl_vect_mult_scalar(volatilities, divVol);

    PnlMat* corrMat = pnl_mat_create(market->getNumOfActions(), market->getNumOfActions());
    Utils::correlationMatrix(path, corrMat);
    pnl_mat_mult_double(corrMat, 1.);

    // creation de performance

    PnlMat* ratesPath = pnl_mat_create(1,1);
    rates->fillPathMat(ratesPath, refDate, initialWindow);
    PnlVect* volRates = pnl_vect_create(rates->getNumOfActions());
    
    Utils::volsOnMat(volRates, ratesPath);
 
    pnl_vect_mult_scalar(volRates, divVol);

    PnlMat* corrRatesMat = pnl_mat_create(rates->getNumOfActions(), rates->getNumOfActions());
    Utils::correlationMatrix(ratesPath, corrRatesMat);

    PnlVect* divRates = pnl_vect_create(market->getNumOfActions());
    int const nbAction = market->getNumOfActions();
    
    int country[30] = {7, 4, 4, 7, 7, 4,4,2,1, 4,4,4, 4, 4,4, 4,4,4,4,3,4,4,4,4,2,7,7,0,4,4};

//On a maintenant la part fixe du div ( rd - rf)


    for(int i = 0; i < divRates->size; i++){
        if (country[i] == 7){
            LET(divRates, i) = 0.;
        } else if (country[i] == 4){
            //c'est de l'usd
            LET(divRates, i) =  - rPerCountry["USD"];

        } else if (country[i] == 2){
            LET(divRates, i) =  - rPerCountry["JAP"];
        }else if (country[i] == 3)
        {
            LET(divRates, i) =  - rPerCountry["MXN"];
        }else if (country[i] == 1){
            LET(divRates, i) =  - rPerCountry["CAD"];
        }else if(country[i] == 0){
            LET(divRates, i) = - rPerCountry["GBP"];
        }
    }
    double arrayexp[5] = {exp(rPerCountry["GBP"]), exp(rPerCountry["CAD"]), exp(rPerCountry["JAP"]), exp(rPerCountry["MXN"]), exp(rPerCountry["USD"])};
    PnlVect* vectexp = pnl_vect_create_from_ptr(5, arrayexp);

    PnlVect* divForStocks = pnl_vect_create(market->getNumOfActions());
    pnl_vect_clone(divForStocks, divRates);

    for (int i = 0; i < divRates->size; i++){
        if (country[i] == 7){
            LET(divForStocks, i) = 0.;
        }else{
            LET(divForStocks, i) = GET(divForStocks, i) + GET(volatilities, i) * GET(volRates, country[i]);
        }
    }

    vector<string> Pdates = 
     { "2015-01-12", "2015-07-13"
     , "2016-01-11", "2016-07-11"
     , "2017-01-11", "2017-07-11"
     , "2018-01-11", "2018-07-11"
     , "2019-01-11", "2019-07-11"
     , "2020-01-11", "2020-07-11"
     , "2021-01-11", "2021-07-11"
     , "2022-01-11", "2022-07-11"};



    Performance *perf = new Performance(Pdates, nbTimeSteps, market, country);

    perf->niveauInitial();


    PnlVect* spots = perf->getNivInitAct();

    PnlVect* spotsRates = pnl_vect_create(rates->getNumOfActions());
    PnlVect * add = pnl_vect_create(rates->getNumOfActions());

    rates->getSpotsFromDate(spotsRates, "2014-07-11");
    rates->getSpotsFromDate(add, "2014-07-15");
    
    pnl_vect_plus_vect(spotsRates, add);
    
    rates->getSpotsFromDate(spotsRates, "2014-07-16");
    pnl_vect_plus_vect(spotsRates, add);

    pnl_vect_div_scalar(spotsRates, 3.);

    for(int i = 0; i < market->getNumOfActions(); i++){

        if( country[i] != 7 ){
            LET(spots, i) = GET(spots,i) * GET(spotsRates, country[i]);
        }
    }



    
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(), rPerCountry["EUR"]  ,1,volatilities, spots);
    pnl_mat_chol(corrMat);

    bs->correlations_ = corrMat;

    BlackScholesModel *bsRates = new BlackScholesModel(rates->getNumOfActions(), rPerCountry["EUR"], 1, volRates, spotsRates );
    pnl_mat_chol(corrRatesMat);
 
    bsRates->correlations_ = corrRatesMat;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    
    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng, bsRates);


    PnlVect* delta = pnl_vect_create(market->getNumOfActions());
    PnlVect* deltaChange = pnl_vect_create(rates->getNumOfActions());
    PnlVect* std_dev_delta = pnl_vect_create(market->getNumOfActions());


   
    //on va maintenant creer sx ce qui va nous donner notre path que l'on pourrat bien simuler sous proba risque neutre!!!

    PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
    PnlVect * vecteurRates = pnl_vect_create(rates->getNumOfActions());
    int i = 0;
    int nconstDate = 0;
    while(nconstDate < Pdates.size() && Pdates[nconstDate] < endFinishDate) {nconstDate++;

    
    }

     PnlMat *past = pnl_mat_create(nconstDate +1, market->getNumOfActions());

 

    pnl_mat_set_row(past, spots, 0);

    PnlMat* pastRates = pnl_mat_create(nconstDate+1, rates->getNumOfActions());
    pnl_mat_set_row(pastRates, spotsRates ,0 );
    for ( i = 1 ; i <= nconstDate; i++) {

        market->getSpotsFromDate(vecteurPast, Pdates[i- 1]);
        rates->getSpotsFromDate( vecteurRates,Pdates[i-1] );
        pnl_mat_set_row(pastRates, vecteurRates, i);


        //on doit remplir maintenant un vecteur de taille équivalente avec les bons taux au bon endroit

        for(int j = 0; j < market->getNumOfActions(); j++){
            MLET(past, i, j) = GET(vecteurPast, j);
            if (country[j] != 7){
            MLET(past, i, j) = MGET(past, i, j)* GET(vecteurRates, country[j]);
            }
        }

    }

    double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();


    mc->delta(past , t,delta, std_dev_delta, divForStocks, deltaChange, divRates, country, pastRates);

    pnl_vect_print(delta);




    pnl_vect_free(&volatilities);
    pnl_mat_free(&path); 
    // on a pas le mme price en t si on calcule price en 0 avant ou pas 
    // le calcul de price en t ne marche pas si on a fait le calcul de price en 0 avant et qu'on reutilise les mm mc et perf
}