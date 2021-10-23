#include "crc_tables.h"
#include <napi.h>

#include <functional>
#include <limits>
#include <string_view>
#include <type_traits>

#include <iostream>
#include <chrono>


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

int main()
{
       using namespace std::literals;

       
       std::cout << "main" << std::endl;
       std::cout << (crc_reflected("\x00\x00\x00\x00"sv, tables::crc32_F4ACFB13) == 0x6FB32240) << std::endl;
       std::cout << (crc_reflected("123456789"sv, tables::crc32_F4ACFB13) == 0x1697D06A) << std::endl;

       std::cout << (crc_reflected("123456789", tables::crc32_ethernet) == 0xCBF43926) << std::endl;
       std::cout << (crc_reflected("123456789", tables::crc64_ECMA) == 0x995DC9BBDF1939FA) << std::endl;

       std::cout << (crc16("123456789") == 0x29B1) << std::endl;

       std::cout << (crc8("123456789", tables::crc8_SAE_J1850) == 0x4B) << std::endl;
       std::cout << (crc8("\x00\x00\x00\x00"sv, tables::crc8_SAE_J1850) == 0x59) << std::endl;

       std::cout << (crc8("123456789", tables::crc8_h2f) == 0xDF) << std::endl;

       //std::cout << crc_reflected<decltype(tables::crc64_ECMA)>("123456789", &tables::crc64_ECMA) << std::endl;
       std::cout << crc_reflected("123456789", tables::crc64_ECMA) << std::endl;
       //std::cout << crc64("123456789") << std::endl;


      /* int64_t total = 0;

       auto const t0 = std::chrono::high_resolution_clock::now();
       for(int i=0; i<200'000; ++i)
       {
              total += crc64("123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789");

       }


       auto const t1 = std::chrono::high_resolution_clock::now();

       for(int i=0; i<200'000; ++i)
       {
              total += crcTptr<uint64_t>("123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789", tables::crc64_ECMA);

       }

       auto const t2 = std::chrono::high_resolution_clock::now();



       for(int i=0; i<200'000; ++i)
       {
              total += crc_reflected<uint64_t>("123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789", tables::crc64_ECMA);

              

       }

       auto const t3 = std::chrono::high_resolution_clock::now();

       auto const ms = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);
       auto const ms2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1);
       auto const ms3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t3-t2);

       std::cout << "total " << total << std::endl;
       std::cout << ms.count() << " vs " << ms2.count() << " vs " << ms3.count() << std::endl;*/
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
