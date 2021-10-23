#include <string_view>
#include <iostream>
#include "crc_tables.h"

#include <chrono>
#include <limits>
#include <type_traits>

// crc8_SAE_J1850
uint8_t crc8(std::string_view s) {
       uint8_t crc = 0xFF;

       for (uint8_t const& ch : s) {
              crc = tables::crc8_SAE_J1850[ch ^ crc];
       }

       return ~crc;
}

// crc8_h2f
uint8_t crc8H2F(std::string_view s) {
       uint8_t crc = 0xFF;

       for (uint8_t const& ch : s) {
              crc = tables::crc8_h2f[ch ^ crc];
       }

       return ~crc;
}

// 16-bit CCITT-FALSE CRC16
uint16_t crc16(std::string_view s) {

       uint16_t crc = 0xFFFF;

       for (uint16_t const& ch : s) {
              crc = tables::crc16_CCITT_FALSE[((crc >> 8) ^ ch) & 0xFFU] ^ (crc << 8U);
       }

       return crc;
}

uint32_t crc32(std::string_view s) {

       uint32_t crc = 0xFFFFFFFF;

       for (uint32_t const& ch : s) {
              crc = tables::crc32_ethernet[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
       }

       return ~crc;
}

uint32_t crc32P4(std::string_view s) {

       uint32_t crc = 0xFFFFFFFF;

       for (uint32_t const& ch : s) {
              crc = tables::crc32_F4ACFB13[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
       }

       return ~crc;
}

uint64_t crc64(std::string_view s) {

       uint64_t crc =  0xFFFFFFFFFFFFFFFF;

       for (uint64_t const& ch : s) {
              crc = tables::crc64_ECMA[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
       }

       return ~crc;
}

template<typename T>
typename std::remove_all_extents<T>::type
       crc_reflected(std::string_view s, T const* const crc_table)
{
       using Tlocal = typename std::remove_all_extents<T>::type;
       Tlocal crc =  std::numeric_limits<Tlocal>::max();

       for (Tlocal const& ch : s) {
              crc = crc_table[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
       }

       return ~crc;  
}


int main()
{
       using namespace std::literals;
       std::cout << "main" << std::endl;
       std::cout << (crc32P4("\x00\x00\x00\x00"sv) == 0x6FB32240) << std::endl;
       std::cout << (crc32P4("123456789") == 0x1697D06A) << std::endl;

       std::cout << (crc_reflected("123456789", tables::crc32_ethernet) == 0xCBF43926) << std::endl;
       std::cout << (crc64("123456789") == 0x995DC9BBDF1939FA) << std::endl;

       std::cout << (crc16("123456789") == 0x29B1) << std::endl;

       std::cout << (crc8("123456789") == 0x4B) << std::endl;
       std::cout << (crc8("\x00\x00\x00\x00"sv) == 0x59) << std::endl;

       std::cout << (crc8H2F("123456789") == 0xDF) << std::endl;

       //std::cout << crc_reflected<decltype(tables::crc64_ECMA)>("123456789", &tables::crc64_ECMA) << std::endl;
       std::cout << crc_reflected("123456789", tables::crc64_ECMA) << std::endl;
       std::cout << crc64("123456789") << std::endl;


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


/*#include <napi.h>


Napi::Value Add(const Napi::CallbackInfo& info) {
     Napi::Env env = info.Env();

       using namespace std::literals;
       std::cout << "main" << std::endl;
       std::cout << (crc32p4("\x00\x00\x00\x00"sv) == 0x6FB32240) << std::endl;
       std::cout << (crc32p4("123456789") == 0x1697D06A) << std::endl;

       std::cout << (crc32("123456789") == 0xCBF43926) << std::endl;
       std::cout << (crc64("123456789") == 0x995DC9BBDF1939FA) << std::endl;

       std::cout << (crc16("123456789") == 0x29B1) << std::endl;

       std::cout << (crc8("123456789") == 0x4B) << std::endl;
       std::cout << (crc8("\x00\x00\x00\x00"sv) == 0x59) << std::endl;

       std::cout << (crc8_h2f("123456789") == 0xDF) << std::endl;

         if (info.Length() !=  1) {
           Napi::TypeError::New(env, "Wrong number of arguments")
               .ThrowAsJavaScriptException();
           return env.Null();
         }

     if(info[0].IsString()) {
            Napi::String str = info[0].As<Napi::String>();
  //auto const sv = std::string_view(reinterpret_cast<const char*>(buf.Data()), buf.ByteLength());

            return Napi::Number::New(env, crc8(str.Utf8Value()));


     } else if(info[0].IsTypedArray()) {
          Napi::TypedArray const arr = info[0].As<Napi::TypedArray>();
          Napi::ArrayBuffer buf = arr.ArrayBuffer();

          auto const sv = std::string_view(reinterpret_cast<const char*>(buf.Data()), buf.ByteLength());
          return Napi::Number::New(env, crc8(sv));
   }
   Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
   auto const sv = std::string_view(reinterpret_cast<const char*>(buf.Data()), buf.ByteLength());

   return Napi::Number::New(env, crc8(sv));

  //return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
     exports.Set(Napi::String::New(env, "crc8"), Napi::Function::New(env, Add));
     return exports;
}

NODE_API_MODULE(crc_addon, Init)
*/
