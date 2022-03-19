#include "MyPnlObject.hpp"
#include <uv.h>
#include <thread>
#include <chrono>



    MyPnlObject::MyPnlObject(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<MyPnlObject>(info) {
            pathfile = info[0].ToString();
        };
            



    Napi::Value MyPnlObject::CalculPnl(const Napi::CallbackInfo& info) {


        Napi::Env env = info.Env();


        spots.clear();
        dates.clear();
        prices.clear();
 

                        // Vanilla Call's information
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

            // For test starting in date t
            double t = 0.5;

            // data for the theorical price
            double  Nd1 = .63455805571885;
            double Nd2 = .557230023739951; 

            // For test starting in date t
            double  Nd1t = .596078398587738;
            double Nd2t = .540537533000572; 

            // variables used to test the 95% proportion
            int compteurJuste = 0;
            int compteurGlobal = 5000;
            bool juste;
            double proportion;
            double precision = 0.01;
            double precisionPnL = 0.2;


            PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
            pnl_rng_sseed(rng, time(NULL));

            PnlVect *sigma = pnl_vect_create_from_scalar(1, 0.2);
            PnlVect *spot = pnl_vect_create_from_scalar(1, 100);

            Derivative *vanille = new VanillaCall(T, nbTimeSteps, K);
            BlackScholesModel *bs = new BlackScholesModel(size, r, rho, sigma, spot);
            MonteCarlo *mc = new MonteCarlo(bs, vanille, fdStep, nbSamples, rng);

            ParseCsv *parser = new ParseCsv(pathfile, 366, 1);

            double prix, stddev;

            mc->price(prix, stddev);
  
            mc->pAndL(nbHedgeDate, errorHedge, parser->marketData_, prix, spots, prices, dates);
            cout << "Nombre de rebalancement : " << nbHedgeDate << endl; 
            cout << "Erreur : " << errorHedge << endl;

            delete parser;
            delete vanille;
            delete bs;
            delete mc;

            pnl_vect_free(&spot);
            pnl_vect_free(&sigma);
            pnl_rng_free(&rng);

            return Napi::Value();
    }

    Napi::Value MyPnlObject::Simple(const Napi::CallbackInfo& info) {


        Napi::Env env = info.Env();


        spots.clear();
        dates.clear();
        prices.clear();

        string directoryToDATA = info[0].ToString();

        double r = 0.04879;
        double fdstep = .01;
        int nbSample = 5000;

        double prix, std_dev;
        double prixt;

        int nbDatesInPast = 15;

        int H = 17;
        double T = 1;

        double errorHedge;



        ParseYahooCsv *parser = new ParseYahooCsv();
        MarketData *market = new MarketData();
        market->fillData(parser, directoryToDATA); // on remplit le dictionnaire avec les données du csv

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
        PnlVect* spotsInit = perf->getNivInitAct();
        // pnl_mat_get_row(spots, path, 0);
        BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(),r,1,volatilities, spotsInit);
        pnl_mat_chol(corrMat);
        bs->correlations_ = corrMat;

        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));


        MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng);

        mc->price(prix, std_dev); 
        std::cout << "Prix en 0 " << prix <<std::endl;


        //calcul du prix en t = auj

        vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-15");

        vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", "2021-12-15");

        PnlMat *pathFull = pnl_mat_new();
        market->getPathFromDates(pathFull, datesFrom2014ToToday);


        PnlMat *past = pnl_mat_create(nbDatesInPast, market->getNumOfActions());


        pnl_mat_set_row(past, perf->getNivInitAct(), 0);

        PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
        for (int i = 0; i < nbDatesInPast - 1; i ++) {
            market->getSpotsFromDate(vecteurPast, Pdates[i]);
            pnl_mat_set_row(past, vecteurPast, i+1);
        }


        double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();
        mc->price(past, t, prixt, std_dev);

        cout << "prix auj " << prixt <<endl;


        //calcul P&L
        

        pnl_mat_resize(path, 17, market->getNumOfActions());
        PnlVect* vectline = pnl_vect_new();
        for(int i = 0; i < past->m; i++){
            pnl_mat_get_row(vectline, past, i);
            pnl_mat_set_row(path, vectline ,i);
        }
        PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), r);
        bs->simul_market(past, T, rng, trend, H - 1, path);
        
        mc->pAndL(H - 1, errorHedge, path, 1.0, spots, prices, dates);

        pnl_vect_free(&volatilities);
        pnl_mat_free(&path); 
        pnl_mat_free(&pathFull);
        pnl_rng_free(&rng);
        pnl_vect_free(&trend);
        pnl_mat_free(&past);
        pnl_vect_free(&vecteurPast);
        pnl_vect_free(&vectline);

        delete mc;
        delete bs;
        delete perf;
        delete parser;
        delete market;

        return Napi::Value();
    }


    Napi::Value MyPnlObject::SimpleRates(const Napi::CallbackInfo& info) {
        
        // Contexte

        Napi::Env env = info.Env();


        // Réinitialisation des valeurs
        spots.clear();
        dates.clear();
        prices.clear();

        string pathFiles = info[0].ToString();
        //string pathFiles = "../data/RATE"; // directoryToRATE
        
        string pathData = info[1].ToString(); // directoryToDATA
        //string pathData = "../data/DATA"; // directoryToDATA

        int H = info[2].As<Napi::Number>().Int32Value();
        //int H = 33; // Nombre de rebalancement

    
        int initialWindow = info[3].As<Napi::Number>().Int32Value();

        string refDate = info[4].ToString(); // Date vers laquelle on veut créer notre fenêtre
        //string refDate = "2021-12-10"; // Date vers laquelle on veut créer notre fenêtre

        string endFinishDate = info[5].ToString(); // Date jusqu'à laquelle on récupère les données historiques
        // Ne pas pas être supérieur à Aujourd'hui - 2 jours 
        
        double fdstep = .01; // 1+h pour les deltas

        int nbTimeSteps= 17; // Ne changerons jamais
        double T = 1;

        int nbSample = info[6].As<Napi::Number>().Int32Value();

        bool simulated = info[7].As<Napi::Boolean>();

        
        map<string, double> rPerCountry = { {"EUR", 0./100.}, {"USD", 0.5/100.}, {"JAP", -0.1/100.}, {"GBP", 0.75/100}, {"CHF", -0.75/100}, {"BRZ", 11.75/100} , {"CAD", 0.5/100}, {"MXN", 6/100}};
    
    // Facteur multiplié à la vol annuelle 
        double divVol = sqrt(8. * 252./(nbTimeSteps-1));
        
        // int nDayStep = 252*8/( H - 1 );
        vector<string> datesFrom2014ToToday = Date::getListOfDates("2014-07-11", endFinishDate);
        vector<string> datesFrom2014To2022 = Date::getListOfDates("2014-07-11", "2022-07-11");
        // int nDatesToSim = (datesFrom2014To2022.size() )/  nDayStep;
        // int nDatesSimed = datesFrom2014ToToday.size()/ nDayStep;

        int nDatesInPast = datesFrom2014ToToday.size() * H / datesFrom2014To2022.size();

        int nDayStep = datesFrom2014To2022.size() / H;

        if (simulated) nDatesInPast = 1;

        //int nbDatesInPast = H - nDatesToSim + nDatesSimed;
        //on calcul de combien on doit avancer 
        

        ParseYahooCsv *parser = new ParseYahooCsv();
        MarketData *market = new MarketData();
        market->fillData(parser, pathData); // on remplit le dictionnaire avec les données du csv
        MarketData *rates = new MarketData();
        
        rates->fillData(parser, pathFiles);
        // for(int i = 0; i < rates->actions.size(); i++){
        //     cout<< rates->actions[i] << endl;
        // }
        // assert(1==2);
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
        double arrayexp[5] = {exp(-rPerCountry["GBP"]), exp(-rPerCountry["CAD"]), exp(-rPerCountry["JAP"]), exp(-rPerCountry["MXN"]), exp(-rPerCountry["USD"])};
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

        Performance *perf = new Performance(Pdates, nbTimeSteps, market, country);

        perf->niveauInitial();

        // vector<string>  dates = Date::getListOfDates("2021-10-12", "2022-12-12");
        //PnlVect* volsim = pnl_vect_create_from_scalar(market->getNumOfActions(), 2);
        PnlVect* spotsStock = perf->getNivInitAct();

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
                LET(spotsStock, i) = GET(spotsStock,i) * GET(spotsRates, country[i]);
            }
        }



        
        // pnl_mat_get_row(spots, path, 0);
        BlackScholesModel *bs = new BlackScholesModel(market->getNumOfActions(), rPerCountry["EUR"]  ,1,volatilities, spotsStock);
        pnl_mat_chol(corrMat);

        bs->correlations_ = corrMat;

        //rates->getSpotsFromDate( spotsRates,Pdates[0] );
        BlackScholesModel *bsRates = new BlackScholesModel(rates->getNumOfActions(), rPerCountry["EUR"], 1, volRates, spotsRates );
        pnl_mat_chol(corrRatesMat);
    
        bsRates->correlations_ = corrRatesMat;

        PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
        pnl_rng_sseed(rng, time(NULL));

        
        MonteCarlo *mc = new MonteCarlo(bs, perf,fdstep,nbSample, rng, bsRates);

        double prix, std_dev;

        mc->price(prix, std_dev, divForStocks, divRates); 

        std::cout << "Prix en 0 " << prix <<std::endl;
        PnlVect* delta = pnl_vect_create(market->getNumOfActions());
        PnlVect* deltaChange = pnl_vect_create(rates->getNumOfActions());
        PnlVect* std_dev_delta = pnl_vect_create(market->getNumOfActions());
        mc->delta(delta, std_dev_delta, divForStocks, deltaChange, divRates, country);

        double prixt;

        //calcul du prix en t = auj



        
        
        PnlMat *pathFull = pnl_mat_new();
        market->getPathFromDates(pathFull, datesFrom2014ToToday);

        PnlMat *past = pnl_mat_create(nDatesInPast, market->getNumOfActions());

    

        pnl_mat_set_row(past, spotsStock, 0);

        PnlMat* pastRates = pnl_mat_create(nDatesInPast, rates->getNumOfActions());
        pnl_mat_set_row(pastRates, spotsRates ,0 );
        //on va maintenant creer sx ce qui va nous donner notre path que l'on pourrat bien simuler sous proba risque neutre!!!

        PnlVect * vecteurPast = pnl_vect_create(market->getNumOfActions());
        PnlVect * vecteurRates = pnl_vect_create(rates->getNumOfActions());
        for (int i = 0; i < nDatesInPast - 1; i ++) {

            market->getSpotsFromDate(vecteurPast, datesFrom2014ToToday[(i+1) * nDayStep]);

            rates->getSpotsFromDate( vecteurRates,datesFrom2014ToToday[(i+1) * nDayStep] );
            pnl_mat_set_row(pastRates, vecteurRates, i+1);


            //on doit remplir maintenant un vecteur de taille équivalente avec les bons taux au bon endroit

            for(int j = 0; j < market->getNumOfActions(); j++){
                MLET(past, i+1, j) = GET(vecteurPast, j);
                if (country[j] != 7){
                MLET(past, i+1, j) = MGET(past, i+1, j)* GET(vecteurRates, country[j]);
                }
            }

        }
        
        double t = (double)(datesFrom2014ToToday.size()-1)/datesFrom2014To2022.size();


        pnl_mat_resize(path, H, market->getNumOfActions());

        PnlMat* pathRates = pnl_mat_create(H , rates->getNumOfActions());

        PnlVect *trend = pnl_vect_create_from_scalar(market->getNumOfActions(), rPerCountry["EUR"]);

        pnl_mat_set_row(past, spotsStock, 0);

        bs->simul_market(past, T, rng, trend, H - 1, path);
        bsRates->simul_market(pastRates, T, rng, trend, H-1, pathRates);

        double res = 0.0;
        if (simulated) {
            res = perf->payoff(path, pathRates);
            while (res < 1.1)  {
                bs->simul_market(past, T, rng, trend, H - 1, path);
                res = perf->payoff(path, pathRates);
            }

        }

        

        double errorHedge;


        mc->pAndL(H - 1, errorHedge, path, 1, pathRates, divForStocks, divRates, country, vectexp, spots, prices, dates);



        datesString.clear();
        for(int i = 0; i < H; i++) {
            datesString.push_back(datesFrom2014To2022[(int) ((datesFrom2014To2022.size() - 1) * i / (H - 1))]);
        }

        pnl_vect_free(&volatilities);
        pnl_mat_free(&path); 
        pnl_mat_free(&pathFull);
        
        return Napi::Value();
    }


    Napi::Value MyPnlObject::Spots(const Napi::CallbackInfo& info) {
        
        Napi::Env env = info.Env();
        Napi::Array outputArray = Napi::Array::New(env, spots.size());

        for (int i = 0; i < spots.size(); i++) {
            outputArray[i] = Napi::Number::New(env, spots[i]);

        }
        return outputArray;
    }

    Napi::Value MyPnlObject::Prices(const Napi::CallbackInfo& info) {
        
        Napi::Env env = info.Env();
        Napi::Array outputArray = Napi::Array::New(env, prices.size());

        for (int i = 0; i < spots.size(); i++) {
            outputArray[i] = Napi::Number::New(env, prices[i]);

        }
        return outputArray;
    }

    Napi::Value MyPnlObject::Dates(const Napi::CallbackInfo& info) {
        
        Napi::Env env = info.Env();       
        Napi::Array outputArray = Napi::Array::New(env, dates.size());

        for (int i = 0; i < spots.size(); i++) {
            outputArray[i] = Napi::Number::New(env, dates[i]);

        }
        return outputArray;
    }

        Napi::Value MyPnlObject::DatesString(const Napi::CallbackInfo& info) {
        
        Napi::Env env = info.Env();       
        Napi::Array outputArray = Napi::Array::New(env, datesString.size());

        for (int i = 0; i < datesString.size(); i++) {
            outputArray[i] = Napi::String::New(env, datesString[i]);

        }
        return outputArray;
    }



    Napi::Object MyPnlObject::Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "MyPnlObject", {
            InstanceMethod("Spots", &MyPnlObject::Spots),
            InstanceMethod("Dates", &MyPnlObject::Dates),
            InstanceMethod("DatesString", &MyPnlObject::DatesString),
            InstanceMethod("Prices", &MyPnlObject::Prices),
            InstanceMethod("CalculPnl", &MyPnlObject::CalculPnl),
            InstanceMethod("Simple", &MyPnlObject::Simple),
            InstanceMethod("SimpleRates", &MyPnlObject::SimpleRates)

        });

        Napi::FunctionReference* constructor = new Napi::FunctionReference();
        *constructor = Napi::Persistent(func);
        env.SetInstanceData(constructor); //NOTE: this assumes only 1 class is exported
                                    //for multiple exported classes, need a struct or other mechanism

        exports.Set("MyPnlObject", func);
        return exports;
    }

    Napi::Object MyPnlObject::NewInstance(Napi::Env env, Napi::Value arg) {
        Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
        return obj;
    }


