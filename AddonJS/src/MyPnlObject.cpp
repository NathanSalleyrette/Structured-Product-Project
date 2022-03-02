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



    Napi::Object MyPnlObject::Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "MyPnlObject", {
            InstanceMethod("Spots", &MyPnlObject::Spots),
            InstanceMethod("Dates", &MyPnlObject::Dates),
            InstanceMethod("Prices", &MyPnlObject::Prices),
            InstanceMethod("CalculPnl", &MyPnlObject::CalculPnl),
            InstanceMethod("Simple", &MyPnlObject::Simple)
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


