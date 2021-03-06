#include <string>
#include "utils/MarketData.hpp"
#include "fs/ParseYahooCsv.hpp"
#include "utils/Utils.hpp"
#include "utils/Date.hpp"
#include "models/BlackScholesModel.hpp"
#include "montecarlo/MonteCarlo.hpp"
#include "financialProducts/Performance.hpp"
#include <map>
#include<cstdio>

using namespace std;

int main(int argc, char **argv)
{
    
    map<string, double> rPerCountry = { {"EUR", 2./100.}, {"USD", 3./100.}, {"JAP", 1.5/100.}, {"GBP", 4.50/100}, {"CHF", -0.75/100}, {"BRZ", 9.25/100} , {"CAD", 4.25/100}, {"MXN", 5.5/100}};

    ParseYahooCsv *parser = new ParseYahooCsv();
    MarketData *market = new MarketData();
    market->fillData(parser); // on remplit le dictionnaire avec les données du csv
    MarketData *rates = new MarketData();
    string pathFiles = "../data/RATE";
    rates->fillData(parser, pathFiles);
    // for(int i = 0; i < rates->actions.size(); i++){
    //     cout<< rates->actions[i] << endl;
    // }
    // assert(1==2);
    // calcul de la volatilité et la correlation
    PnlMat* path = pnl_mat_create(1,1);
    market->fillPathMat(path, "2021-12-10", 5);

    PnlVect* volatilities = pnl_vect_create(market->getNumOfActions());
    Utils::volsOnMat(volatilities, path);
    pnl_vect_mult_scalar(volatilities, 1. / 16.);

    PnlMat* corrMat = pnl_mat_create(market->getNumOfActions(), market->getNumOfActions());
    Utils::correlationMatrix(path, corrMat);
    pnl_mat_mult_double(corrMat, 1.);

    // creation de performance

    PnlMat* ratesPath = pnl_mat_create(1,1);
    rates->fillPathMat(ratesPath, "2021-12-10", 5);
    
    PnlVect* volRates = pnl_vect_create(rates->getNumOfActions());

    Utils::volsOnMat(volRates, ratesPath);
    pnl_vect_mult_scalar(volRates, 1./16.);

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

    // Performance *perf = new Performance(observeDates, market, dates);
    Performance *perf = new Performance(Pdates, market, country);

    perf->niveauInitial();

    // vector<string>  dates = Date::getListOfDates("2021-10-12", "2022-12-12");
    //PnlVect* volsim = pnl_vect_create_from_scalar(market->getNumOfActions(), 2);
    PnlVect* spots = perf->getNivInitAct();

    PnlVect* spotsRates = pnl_vect_create(rates->getNumOfActions());
    PnlVect * add = pnl_vect_create(rates->getNumOfActions());

    rates->getSpotsFromDate(spotsRates, "2014-07-11");
    rates->getSpotsFromDate(add, "2014-07-15");
    
    pnl_vect_plus_vect(spotsRates, add);
    
    rates->getSpotsFromDate(spotsRates, "2014-07-16");
    pnl_vect_plus_vect(spotsRates, add);

    pnl_vect_div_scalar(spotsRates, 3.);
    //  pnl_vect_print(spots);
    //  cout << "-----"<<endl;
    for(int i = 0; i < market->getNumOfActions(); i++){

        if( country[i] != 7 ){
            LET(spots, i) = GET(spots,i) * GET(spotsRates, country[i]);
        }
    }
    // pnl_vect_print(spots);
   // assert(1==2);
    //pnl_vect_inv_term(spotsRates);


    
    // pnl_mat_get_row(spots, path, 0);
    BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(), rPerCountry["EUR"]  ,1,volatilities, spots);
    pnl_mat_chol(corrMat);

    bs->correlations_ = corrMat;

    rates->getSpotsFromDate( spotsRates,Pdates[0] );
    BlackScholesModel *bsRates = new BlackScholesModel(rates->getNumOfActions(), rPerCountry["EUR"], 1, volRates, spotsRates );
    pnl_mat_chol(corrRatesMat);
 
    bsRates->correlations_ = corrRatesMat;

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    double fdstep = .01;
    int nbSample = 5000;
    MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng, bsRates);

    double prix, std_dev;

    // mc->price(prix, std_dev);
    // PnlVect* delta = pnl_vect_create(market->getNumOfActions());
    // PnlVect* std_dev_delta = pnl_vect_create(market->getNumOfActions());
    // mc->delta(delta, std_dev_delta);
    // pnl_vect_print(delta);
    mc->price(prix, std_dev, divForStocks, divRates); 

    std::cout << "Prix en 0 " << prix <<std::endl;
    PnlVect* delta = pnl_vect_create(market->getNumOfActions());
    PnlVect* deltaChange = pnl_vect_create(rates->getNumOfActions());
    PnlVect* std_dev_delta = pnl_vect_create(market->getNumOfActions());
    mc->delta(delta, std_dev_delta, divForStocks, deltaChange, divRates, country);
cout<< "------------------------"<<endl;
pnl_vect_print(deltaChange);

cout<< "------------------------"<<endl;
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

    pnl_mat_set_row(past, spots, 0);

    PnlMat* pastRates = pnl_mat_create(nbDatesInPast, rates->getNumOfActions());
    pnl_mat_set_row(pastRates, spotsRates ,0 );
    //on va maintenant creer sx ce qui va nous donner notre path que l'on pourrat bien simuler sous proba risque neutre!!!

    PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
    PnlVect * vecteurRates = pnl_vect_create(rates->getNumOfActions());
    for (int i = 0; i < nbDatesInPast - 1; i ++) {
        market->getSpotsFromDate(vecteurPast, Pdates[i]);
        // pnl_mat_set_row(past, vecteurPast, i+1);
        rates->getSpotsFromDate( vecteurRates,Pdates[i] );
        pnl_mat_set_row(pastRates, vecteurRates, i+1);


        //on doit remplir maintenant un vecteur de taille équivalente avec les bons taux au bon endroit

        for(int j = 0; j < market->getNumOfActions(); j++){
            MLET(past, i+1, j) = GET(vecteurPast, j);
            if (country[j] != 7){
            MLET(past, i+1, j) = MGET(past, i+1, j)* GET(vecteurRates, country[j]);
            }
        }

        //pnl_vect_mult_vect_term(vecteurPast, vecteurRates);
        //pnl_mat_set_row(past, vecteurPast , i+1);
    }
    
    // for(int i = 0; i < pastRates->m; i++){
    //     pnl_mat_get_row(vecteurRates,pastRates, i);
    //     pnl_vect_inv_term(vecteurRates);
    //     pnl_mat_set_row(pastRates, vecteurRates  ,i);
    // }
    // pnl_mat_print(pastRates);

    double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();
    mc->price(past, t, prixt, std_dev, divForStocks, divRates, pastRates);

    cout << "prix auj " << prixt <<endl;

    mc->delta(past , t,delta, std_dev_delta, divForStocks, deltaChange, divRates, country, pastRates);
cout<< "------------------------"<<endl;
pnl_vect_print(deltaChange);

cout<< "------------------------"<<endl;


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

    PnlMat* pathRates = pnl_mat_create(17 , rates->getNumOfActions());

    PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), rPerCountry["EUR"]);
    bs->simul_market(past, T, rng, trend, H - 1, path);
    bsRates->simul_market(pastRates, T, rng, trend, H-1, pathRates);
    


    

    double errorHedge;

    mc->pAndL(H - 1, errorHedge, path, 1, pathRates, divForStocks, divRates, country,vectexp);


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
    // on a pas le mme price en t si on calcule price en 0 avant ou pas 
    // le calcul de price en t ne marche pas si on a fait le calcul de price en 0 avant et qu'on reutilise les mm mc et perf
}