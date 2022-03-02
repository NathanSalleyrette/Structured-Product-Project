#include <napi.h>
#include <iostream>
#include "pnl_api.hpp"

void test(int i) {
    std::cout << "Hello " <<  i << std::endl;
}

double * pnl_api::createRandom() {
    static double arr[10];
    test(0);
    PnlVect *result = pnl_vect_create(10);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    pnl_vect_rng_normal(result, result->size, rng);
    for (int i = 0; i < result->size; i++) {
        arr[i] = GET(result, i);
    }
    pnl_vect_free(&result);
    pnl_rng_free(&rng);
    return arr;
}



Napi::Array pnl_api::createRandomWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    test(1);
    double* res = createRandom();

    Napi::Array outputArray = Napi::Array::New(env, 10);


    for (int i = 0; i < 10; i++) {
        outputArray[i] = Napi::Number::New(env, res[i]);

    }

    return outputArray;

}

Napi::Array pnl_api::createRandomPnlInside(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    static double res[10];
    PnlVect *result = pnl_vect_create(10);
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    pnl_vect_rng_normal(result, result->size, rng);

    Napi::Array outputArray = Napi::Array::New(env, 10);


    for (int i = 0; i < 10; i++) {
        outputArray[i] = Napi::Number::New(env, GET(result, i));

    }
    pnl_vect_free(&result);
    pnl_rng_free(&rng);
    return outputArray;

}

Napi::Array pnl_api::createArrayHello(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    test(1);

    Napi::Array outputArray = Napi::Array::New(env, 10);


    for (int i = 0; i < 10; i++) {
        outputArray[i] = Napi::String::New(env, "Hello");

    }

    return outputArray;

}


Napi::Array pnl_api::ArrayNull(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();


    Napi::Array outputArray = Napi::Array::New(env, 0);

    return outputArray;

}


Napi::Object pnl_api::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("pnl", Napi::Function::New(env, pnl_api::createRandomWrapped));
    exports.Set("hello", Napi::Function::New(env, pnl_api::createArrayHello));
    exports.Set("pnl_inside", Napi::Function::New(env, pnl_api::createRandomPnlInside));
    exports.Set("null", Napi::Function::New(env, pnl_api::ArrayNull));

    return exports;
}
