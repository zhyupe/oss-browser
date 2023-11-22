#include <napi.h>
#include "crc64_ecma_182.h"
#include<sstream>
#include<string>

namespace CRC64JS {

static uint64_t ZERO = 0;

Napi::Buffer<uint64_t> CRC64(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Buffer<char> buff = info[0].As<Napi::Buffer<char>>();
    Napi::Buffer<uint64_t> ret;

    if(info.Length() > 1)
    {
        ret = info[1].As<Napi::Buffer<uint64_t>>();
    }
    else
    {
        ret = Napi::Buffer<uint64_t>::New(env, &ZERO, sizeof(ZERO));
    }

    uint64_t* crc = ret.Data();
    void* orig_buff = buff.Data();
    *crc = crc64js_base::crc64(*crc, orig_buff, buff.Length());

    return ret;
}

Napi::String ToUInt64String(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Buffer<uint64_t> ret = info[0].As<Napi::Buffer<uint64_t>>();
  uint64_t* crc = ret.Data();
  char str[32];
  sprintf(str, "%llu", *crc);

  return Napi::String::New(env, str);
}

uint64_t stringToUINT64(const std::string s) {
    std::stringstream a;
    a << s;
    uint64_t ret = 0;
    a >> ret;
    return ret;
}

Napi::String CombineCRC64(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() !=  3) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    if (!info[0].IsString() ||  !info[1].IsString() || !info[2].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    std::string arg0 = info[0].As<Napi::String>();
    uint64_t crc1 = stringToUINT64(arg0);

    std::string arg1 = info[1].As<Napi::String>();
    uint64_t crc2 = stringToUINT64(arg1);

    std::string arg3 = info[2].As<Napi::String>();
    uint64_t len2 = stringToUINT64(arg3);

    crc1 = crc64js_base::crc64_combine(crc1, crc2, len2);

    std::string asString = std::to_string(crc1);

    return Napi::String::New(env, asString);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    crc64js_base::crc64_init();
    exports.Set(Napi::String::New(env, "crc64"), Napi::Function::New(env, CRC64));
    exports.Set(Napi::String::New(env, "toUInt64String"), Napi::Function::New(env, ToUInt64String));
    exports.Set(Napi::String::New(env, "combineCrc64"), Napi::Function::New(env, CombineCRC64));
    return exports;
}

NODE_API_MODULE(crc64, Init)

}
