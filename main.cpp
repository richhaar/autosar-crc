#include <string_view>
#include <iostream>

#include <napi.h>

#include "crc_tables.h"

// crc8_SAE_J1850
uint8_t crc8(std::string_view s) {
       uint8_t crc = 0xFF;

       for (uint8_t const& ch : s) {
              crc = tables::crc8_SAE_J1850[ch ^ crc];
       }

       return ~crc;
}

// crc8_h2f
uint8_t crc8_h2f(std::string_view s) {
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


              /*****************************************************************/
              /*                                                               */
              /* CRC LOOKUP TABLE                                              */
              /* ================                                              */
              /* The following CRC lookup table was generated automagically    */
              /* by the Rocksoft^tm Model CRC Algorithm Table Generation       */
              /* Program V1.0 using the following model parameters:            */
              /*                                                               */
              /*    Width   : 4 bytes.                                         */
              /*    Poly    : 0xF4ACFB13L                                      */
              /*    Reverse : TRUE.                                            */
              /*                                                               */
              /* For more information on the Rocksoft^tm Model CRC Algorithm,  */
              /* see the document titled "A Painless Guide to CRC Error        */
              /* Detection Algorithms" by Ross Williams                        */
              /* (ross@guest.adelaide.edu.au.). This document is likely to be  */
              /* in the FTP archive "ftp.adelaide.edu.au/pub/rocksoft".        */

uint32_t crc32p4(std::string_view s) {

       uint32_t crc = 0xFFFFFFFF;

       for (uint32_t const& ch : s) {
              crc = tables::crc32_F4ACFB13[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
       }

       return ~crc;
}

uint32_t crc32(std::string_view s) {

       uint32_t crc = 0xFFFFFFFF;

       for (uint32_t const& ch : s) {
              crc = tables::crc32_ethernet[(crc ^ ch) & 0xFFU] ^ (crc >> 8U);
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

int main()
{
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

}



Napi::Value Add(const Napi::CallbackInfo& info) {
     Napi::Env env = info.Env();

  /*if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  double arg0 = info[0].As<Napi::Number>().DoubleValue();
  double arg1 = info[1].As<Napi::Number>().DoubleValue();
  Napi::Number num = Napi::Number::New(env, arg0 + arg1);*/

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
