/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-convert_char_to_unsigned_int.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月07日
*   Description ：
================================================================*/
#include <iostream> // cout, endl
#include <sstream> // stringstream

unsigned int cstr_to_unsigned_int(const char* cstr) {
    std::stringstream ss;
    unsigned int value;
    ss << cstr;
    ss >> value;
    return value;
}

int main() {

    char cstr[] = " 123455678";
    std::cout << cstr_to_unsigned_int(cstr) << std::endl; // 123455678
    char cstr2[] = " 00";
    std::cout << cstr_to_unsigned_int(cstr2) << std::endl; // 0

    return 0;
}
