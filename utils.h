#include <iostream>
 #include <iomanip>
 #include <cmath>
 #include <iostream>
 #include <string>
 #include <format>
 #include <cstring>
 #include <cstdint>
 using namespace std;
 
 void insert_in_array(signed char src[8], signed char dest[32], int index){
      for (int i = index; i < index+8; i++)
         {
             dest[i] = src[i-index];
         } 
 }
 
 
 void long_to_bytes(int64_t value, signed char buffer[8]){
     /* for (int i = 0; i < 8; i++)
         {
            buffer[i] = ((value >> (8 * i)) & 0XFF);
         } */
     std::memcpy(buffer, &value, sizeof(value));
 }

 
 double bytes_to_double(const signed char bytes[8]) { //AI-generated
     double value;
     std::memcpy(&value, bytes, sizeof(value));
     return value;
 }
 
 void double_to_bytes(double value, signed char bytes[8]) { //AI-generated
     std::memcpy(bytes, &value, sizeof(value));
 }
 
 double round_to_digits(double value, int digits) { //AI-modified
     double scale = std::pow(10.0, digits);
     return std::round(value * scale) / scale;
 }
 
 signed char define_xored_length(int input_length){
     if(input_length <= 127){
         return  static_cast<signed char>(input_length);
     }
     signed char xored_length = (input_length % 128) != 0 ?   static_cast<signed char>( input_length % 128) :   static_cast<signed char>(127);
     return xored_length;
 }
 
 void string_as_signed_char(string str, signed char dst[]){
     for(int i = 0; i<str.length(); ++i){
         dst[i] = (signed char) str.at(i);
     }
 }
 
 int64_t to_signed_long(signed char bytes[]) { //size assumed to be exactly eight
         int64_t value;
 
         std::memcpy(&value, bytes, sizeof(value));
         return value;
 }
 
 
 void flatten(signed char key[4][8], signed char target_array[32]){
     int l = 0;
     for (int i=0; i<4; ++i) {
         for (int j=0; j<8; ++j) {
             target_array[l] = key[i][j];
             ++l;
         }
     }
 }
 
 
 void get_subarray(signed char src[32], signed char dest[8], int begin, int end ){
     for(int i =0; i<end-begin; ++i){
         dest[i] = src[begin+i];
     }
 
 }
 void get_subarray_from_string(string src, signed char dest[], int begin, int end ){
     for(int i =0; i<end-begin; ++i){
         dest[i] = src.at(begin+i);
     }
 }
 string convert_to_string( signed char* a, int size)
 {
     int i;
     string s = "";
     for (i = 0; i < size; i++) {
         s.push_back(a[i]);
         //s = s + a[i];
     }
     return s;
 }
