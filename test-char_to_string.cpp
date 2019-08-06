/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-char_to_string.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月07日
*   Description ：
================================================================*/
#include <iostream>
#include <string>
#include <sstream>

std::string cstr_to_string(const char* cstr) {
    std::string s;
    std::stringstream ss;
    ss << cstr;
    ss >> s;
    return s;
}

int main() {
    char cstr[] = "我是中国人。";
    std::string s = cstr_to_string(cstr);
    std::cout << s << std::endl;

    return 0;
}
