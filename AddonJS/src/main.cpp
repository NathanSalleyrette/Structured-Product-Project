#include <napi.h>
#include "pnl_api.hpp"
#include "MyPnlObject.hpp"


Napi::Object CreateObject(const Napi::CallbackInfo& info) {
  return MyPnlObject::NewInstance(info.Env(), info[0]);
}


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "createObject"), Napi::Function::New(env, CreateObject));
    pnl_api::Init(env, exports);
    return MyPnlObject::Init(env, exports);;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)