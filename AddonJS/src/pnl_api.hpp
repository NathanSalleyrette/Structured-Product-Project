#include <napi.h>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"


namespace pnl_api {
    double* createRandom();
    Napi::Array createRandomWrapped(const Napi::CallbackInfo& info);
    Napi::Object Init(Napi::Env env, Napi::Object exports);
    Napi::Array createArrayHello(const Napi::CallbackInfo& info);
    Napi::Array createRandomPnlInside(const Napi::CallbackInfo& info);
    Napi::Array ArrayNull(const Napi::CallbackInfo& info);
}