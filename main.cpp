#include "crc_tables.h"
#include <napi.h>

#include <functional>
#include <limits>
#include <string_view>
#include <type_traits>

using namespace std::placeholders;

/**
  Normal crc 8 bit and 16 bit table based calculation
  */
uint8_t crc8(std::string_view const s, uint8_t const* const table) {
    uint8_t crc = 0xFF;

    for (uint8_t const& ch : s) {
        crc = table[ch ^ crc];
    }

    return ~crc;
}

uint16_t crc16(std::string_view const s) {

    uint16_t crc = 0xFFFF;

    for (uint16_t const& ch : s) {
        crc = tables::crc16_CCITT_FALSE[((crc >> 8) ^ ch) & 0xFFU] ^ (crc << 8U);
    }

    return crc;
}

/**
  Reflected crc table based calculation
  */
    template<typename T>
typename std::remove_all_extents<T>::type crc_reflected(std::string_view const s, T const* const crc_table)
{
    using itemT = typename std::remove_all_extents<T>::type;

    itemT crc =  std::numeric_limits<itemT>::max();

    for (itemT const& ch : s) {
        crc = crc_table[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
    }

    return ~crc;  
}

template<typename f>
Napi::Value CallCRC(Napi::CallbackInfo const& info, f func) {
    Napi::Env env = info.Env();

    if (info.Length() !=  1) {
        Napi::TypeError::New(env, "Expected one argument.")
            .ThrowAsJavaScriptException();
        return env.Null();
    }  

    std::string_view sv{};

    if(info[0].IsString()) {
        Napi::String str = info[0].As<Napi::String>();
        sv = str.Utf8Value();
    } else if(info[0].IsTypedArray()) {
        Napi::TypedArray const arr = info[0].As<Napi::TypedArray>();
        Napi::ArrayBuffer buf = arr.ArrayBuffer();
        sv = std::string_view(reinterpret_cast<const char*>(buf.Data()), buf.ByteLength());
    } else if(info[0].IsArrayBuffer()) {
        Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
        sv = std::string_view(reinterpret_cast<const char*>(buf.Data()), buf.ByteLength());
    } else {
        Napi::TypeError::New(env, "Expected String, TypedArray or ArrayBuffer.")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Number::New(env, func(sv));
}

Napi::Value call_crc8_SAE_J1850(Napi::CallbackInfo const& info)
{
    return CallCRC(info, std::bind(crc8, _1, tables::crc8_SAE_J1850));
}

Napi::Value call_crc8_h2f(Napi::CallbackInfo const& info)
{
    return CallCRC(info, std::bind(crc8, _1, tables::crc8_h2f));
}

Napi::Value call_crc16_CCITT_FALSE(Napi::CallbackInfo const& info)
{
    return CallCRC(info, crc16);
}

Napi::Value call_crc32_ethernet(Napi::CallbackInfo const& info)
{
    return CallCRC(info, std::bind(crc_reflected<uint32_t>, _1, tables::crc32_ethernet));
}

Napi::Value call_crc32_F4ACFB13(Napi::CallbackInfo const& info)
{
    return CallCRC(info, std::bind(crc_reflected<uint32_t>, _1, tables::crc32_F4ACFB13));
}

Napi::Value call_crc64_ECMA(Napi::CallbackInfo const& info)
{
    return CallCRC(info, std::bind(crc_reflected<uint64_t>, _1, tables::crc64_ECMA));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "crc8"), Napi::Function::New(env, call_crc8_SAE_J1850));
    exports.Set(Napi::String::New(env, "crc8_h2f"), Napi::Function::New(env, call_crc8_h2f));
    exports.Set(Napi::String::New(env, "crc16"), Napi::Function::New(env, call_crc16_CCITT_FALSE));
    exports.Set(Napi::String::New(env, "crc32"), Napi::Function::New(env, call_crc32_ethernet));
    exports.Set(Napi::String::New(env, "crc32_p4"), Napi::Function::New(env, call_crc32_F4ACFB13));
    exports.Set(Napi::String::New(env, "crc64"), Napi::Function::New(env, call_crc64_ECMA));

    return exports;
}

NODE_API_MODULE(crc_addon, Init)
