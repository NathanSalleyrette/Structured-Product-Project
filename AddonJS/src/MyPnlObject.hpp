#ifndef MYPNLOBJECT_H
#define MYPNLOBJECT_H

#include <napi.h>
#include <iostream>
#include <string>
#include <vector>
#include "montecarlo/MonteCarlo.hpp"
#include "fs/ParseCsv.hpp"
#include "fs/ParseYahooCsv.hpp"
#include "models/BlackScholesModel.hpp"
#include "financialProducts/VanillaCall.hpp"
#include "financialProducts/Performance.hpp"
#include "utils/Utils.hpp"
#include "utils/Date.hpp"




class MyPnlObject : public Napi::ObjectWrap<MyPnlObject> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
        MyPnlObject(const Napi::CallbackInfo& info);
        //Napi::Value CallEmit(const Napi::CallbackInfo& info);




    private:
         string pathfile = "";
        std::vector<double> spots;
        std::vector<double> prices;
        std::vector<double> dates;
        std::vector<std::string> datesString;

        // vecteur des actions
        std::vector<std::string> stocksString;
        std::vector<double> deltasStock;

        Napi::Value CalculPnl(const Napi::CallbackInfo& info);
        Napi::Value Simple(const Napi::CallbackInfo& info);
        Napi::Value SimpleRates(const Napi::CallbackInfo& info);
        Napi::Value ComputeDeltas(const Napi::CallbackInfo& info);

        Napi::Value Deltas(const Napi::CallbackInfo& info);
        Napi::Value Stocks(const Napi::CallbackInfo& info);

        Napi::Value Dates(const Napi::CallbackInfo& info);
        Napi::Value DatesString(const Napi::CallbackInfo& info);
        
        Napi::Value Spots(const Napi::CallbackInfo& info);
        Napi::Value Prices(const Napi::CallbackInfo& info);

};

#endif