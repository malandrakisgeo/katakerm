/*
 *  Katakerm implementation
 *
 *  Copyright (C) 2025 Georgios Malandrakis <malandrakisgeo@gmail.com>
 */

 #include <iostream>
 #include <iomanip>
 #include <cmath>
 #include <iostream>
 #include <string>
 #include <format>
 #include <cstring>
 #include <cstdint>
 using namespace std;
 #include "utils.h"
 #include "katakerm.h"


 void handle_large_input(signed char input[], int input_length,signed char byte_sequence[32] ){
    int parts_of_32_bytes = (input_length / 32);
    signed char temp[4][8] = {};
    get_subarray(input, byte_sequence, 0, 32);
    derive_key(byte_sequence, 32, temp);
    flatten(temp, byte_sequence);

       for (int i = 1; i < parts_of_32_bytes; i++) {

           signed char temporary_array[8] = {};

           get_subarray(byte_sequence,temporary_array, 0, 8);
           int64_t a = to_signed_long(temporary_array) + 1*i;

           get_subarray(byte_sequence,temporary_array, 8, 16);
           int64_t b = to_signed_long(temporary_array )+ 4*i;

           get_subarray(byte_sequence,temporary_array, 16, 24);
           int64_t c = to_signed_long(temporary_array )+ 9*i;

           get_subarray(byte_sequence,temporary_array, 24, 32);
           int64_t d = to_signed_long(temporary_array)+  16*i;

           int l = 0;
           for (int j = i*32; j < i*32 + 32; j++) {
              signed char byty = (signed char) ( i%255);
              signed char temp = (signed char) (byte_sequence[l] ^ byty);
              input[j] ^= temp;
              ++l;
          }

           signed char new_byte_sequence[32] = {};

           get_subarray(input, temporary_array, i * 32, i * 32 + 8);
           int64_t bt1 =  (to_signed_long(temporary_array) + 1) * a;
           long_to_bytes(bt1, temporary_array);
           insert_in_array(temporary_array, new_byte_sequence, 0);

           get_subarray(input, temporary_array, i * 32 + 8, i * 32 + 16);
           int64_t bt2 =  (to_signed_long(temporary_array) + 4) * b;
           long_to_bytes(bt2, temporary_array);
           insert_in_array(temporary_array, new_byte_sequence, 8);

           get_subarray(input, temporary_array, i * 32 + 16, i * 32 + 24);
           int64_t bt3 =  (to_signed_long(temporary_array) + 9) * c;
           long_to_bytes(bt3, temporary_array);
           insert_in_array(temporary_array, new_byte_sequence, 16);


           get_subarray(input, temporary_array, i * 32 + 24, i * 32 + 32);
           int64_t bt4 =  (to_signed_long(temporary_array) + 16) * d;
           long_to_bytes(bt4, temporary_array);
           insert_in_array(temporary_array, new_byte_sequence, 24);

           for (int j = 0; j < 32; j++) {
               byte_sequence[j] ^= new_byte_sequence[j];
           }

       }
       for (int j = 0; j < input_length % 32; j++) {
           byte_sequence[j] ^= input[32 * parts_of_32_bytes + j];
           byte_sequence[j] ^= (signed char) (j & 0xFF);
       }
 }



 void cascaded_xor(signed char byte_sequence[32], signed char xored_length, signed char T[4][8]){
    //byte_sequence length assumed to be 32
 
     int pointer = 0;
     for (int i = 0; i < 4; ++i) {
         for (int j = 0; j < 8; ++j) {
             signed char a = (signed char) ((i + 1) * (j + 1));
             T[i][j] = (signed char) (a ^ byte_sequence[pointer] ^ (xored_length));
             if (j > 0) {
                 T[i][j] ^= (signed char) (T[i][j - 1]);
             } else {
                 if (i > 0) {
                     T[i][j] ^= (signed char) (T[i - 1][7]);
                 }
             }
             ++pointer;
         }
     }
     T[0][0] ^= T[3][7];
 }

 void deduce_longs_from_T_and_multiply(signed char T[4][8], signed char tox [8]){
    signed char bitos[4] = {};

    for (int i = 0; i < 4; ++i) {
        bitos[i] = (signed char) (T[i][0] ^ T[i][1] ^ T[i][6] ^ T[i][7]);
    }

    int64_t first = (int64_t) pow(bitos[0], 1) == 0 ? 1 : (int64_t) pow(bitos[0], 1);
    int64_t second = (int64_t) bitos[1] == 0 ? 2 : (int64_t) pow(bitos[1], 2);
    int64_t third = (int64_t) bitos[2] == 0 ? 4 : (int64_t) pow(bitos[2], 3);
    int64_t fourth = (int64_t) bitos[3] == 0 ? 27 : (int64_t) pow(bitos[3], 4);
    int64_t ginomeno = first * second * third * fourth;


    long_to_bytes(ginomeno, tox);
 }
 

 void derive_key(signed char input[], int input_length, signed char key[4][8]){
 
     signed char byte_sequence[32]  = {};
 
     signed char xored_length = define_xored_length(input_length);
 
     if(input_length<=32){

         for(int i=0; i<32; ++i){
             if(i<input_length){
                 byte_sequence[i] = (signed char)input[i];
             }else{
                 byte_sequence[i] = (signed char)0;
             }
         }
     }
 
 
     if (input_length > 32) {
        handle_large_input(input, input_length, byte_sequence);
     }
     signed char T[4][8] = {}; 

     cascaded_xor(byte_sequence, xored_length, T);

     double tr = round_to_digits(cos(to_signed_long(T[0])), 12) + round_to_digits(sin(to_signed_long(T[1]) + to_signed_long(T[2])), 12);
     double tr2 = round_to_digits(cos(to_signed_long(T[1]) + to_signed_long(T[2])), 12) + round_to_digits(sin(to_signed_long(T[3])), 12);
 
     signed char trigonometric_1[8] = {};
     signed char trigonometric_2[8] = {};
 
     double_to_bytes(tr, trigonometric_1);
     double_to_bytes(tr2, trigonometric_2);
     signed char tox [8]= {};

 
      deduce_longs_from_T_and_multiply(T, tox);

     for (int j = 0; j < 8; ++j) {
         tox[j] = (signed char) (tox[j] ^ trigonometric_1[j] ^ trigonometric_2[j]);
     }
 
     for (int i = 0; i < 4; ++i) {
         signed char position_leftmostbyte_cubed = (signed char) ((i * i * i) & 0xFF);
         for (int j = 0; j < 8; ++j) {
             key[i][j] = (signed char) (T[i][j] ^ tox[j] ^ position_leftmostbyte_cubed);
             std::cout << ((int) key[i][j] ) << ' ';
         }

     }
     std::cout << std::endl << ' ';

 }
 
 int main() {
     string input;
     cin >> input;
     signed char key_array[4][8] = {};
 
     signed char dst[input.length()] = {};
     std::cout << sizeof(dst)  << std::endl;;
 
     string_as_signed_char(input, dst);
 
     //derive_key_new(dst, input.length(), key_array);
     derive_key(dst, input.length(), key_array);
 }
 