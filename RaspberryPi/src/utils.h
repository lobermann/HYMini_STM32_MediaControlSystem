/* 
 * File:   utils.h
 * Author: Lukas
 *
 * Created on 01. Mai 2014, 01:00
 */

#ifndef UTILS_H
#define	UTILS_H

#include <string>

uint16_t color_conv(char r, char g, char b)
{
    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
};

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(3 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.push_back(' ');
    }
    return output;
}

#endif	/* UTILS_H */

