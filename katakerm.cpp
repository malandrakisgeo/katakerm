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


void long_to_bytes(int64_t value, signed char buffer[8]){
    /* for (int i = 0; i < 8; i++)
        {
           buffer[i] = ((value >> (8 * i)) & 0XFF);
        } */
    std::memcpy(buffer, &value, sizeof(value));
}

int64_t bytes_to_long(const signed char bytes[8]) {
    int64_t value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
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


void derive_key(string input, signed char key[4][8]){

    signed char byte_sequence[32]  = {};
    int input_length = input.length();
    signed char xored_length = define_xored_length(input_length);

    if(input_length<=32){
        for(int i=0; i<32; ++i){
            if(i<input_length){
                byte_sequence[i] = (signed char)input.at(i);
            }else{
                byte_sequence[i] = (signed char)0;
            }
        }
    }


    if (input_length > 32) {
        int parts_of_32_bytes = (input_length / 32);
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < parts_of_32_bytes; j++) {
                byte_sequence[i] ^= (signed char)(input.at((32*j) + i));
                byte_sequence[i] ^= (signed char)(j & 0xFF);
            }
        }
        for (int j = 0; j < input_length % 32; j++) {
            byte_sequence[j] ^= input.at(32 * parts_of_32_bytes + j);
            byte_sequence[j] ^= (signed char) (j & 0xFF);
        }
    }


    signed char T[4][8] = {}; ;

    int pointer = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) {
            signed char a = (signed char) ((i + 1) * (j + 1));
            T[i][j] = (signed char) (a ^ byte_sequence[pointer] ^ (xored_length));
            if (j > 0) {
                T[i][j] ^= (signed char) (T[i][j - 1]);
            } else {
                if (i > 0) {
                    T[i][j] ^= (signed char) (T[i - 1][0]);
                }
            }
            ++pointer;
        }
    }
    T[0][0] ^= T[3][7];

    double tr = round_to_digits(cos(bytes_to_long(T[0])), 12) + round_to_digits(sin(bytes_to_long(T[1]) + bytes_to_long(T[2])), 12);
    double tr2 = round_to_digits(cos(bytes_to_long(T[1]) + bytes_to_long(T[2])), 12) + round_to_digits(sin(bytes_to_long(T[3])), 12);

    signed char trigonometric_1[8] = {};
    signed char trigonometric_2[8] = {};

    double_to_bytes(tr, trigonometric_1);
    double_to_bytes(tr2, trigonometric_2);

    signed char bitos[4] = {};


    for (int i = 0; i < 4; ++i) {
        bitos[i] = (signed char) (T[i][0] ^ T[i][1] ^ T[i][6] ^ T[i][7]);
    }

    int64_t first = (int64_t) pow(bitos[0], 1) == 0 ? 1 : (int64_t) pow(bitos[0], 1);
    int64_t second = (int64_t) bitos[1] == 0 ? 2 : (int64_t) pow(bitos[1], 2);
    int64_t third = (int64_t) bitos[2] == 0 ? 4 : (int64_t) pow(bitos[2], 3);
    int64_t fourth = (int64_t) bitos[3] == 0 ? 27 : (int64_t) pow(bitos[3], 4);
    int64_t ginomeno = first * second * third * fourth;

    signed char tox [8]= {};

    long_to_bytes(ginomeno, tox);

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

}


int main() {
    string input;
    cin >> input;
    signed char key_array[4][8] = {};

    derive_key(input, key_array);
}
