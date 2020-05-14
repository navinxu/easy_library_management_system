/*================================================================
*   Copyright (C) 2018 Navin Xu. All rights reserved.
*   
*   Filename    ：test-string_to_int.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2018年10月15日
*   Description ：
================================================================*/
#include <iostream>
#include <string>
#include <sstream>

template <class T>
void convertFromString(T &value, const std::string &s);

int main() {

    std::string astring = "123";
    int i;
    convertFromString(i, astring);
    std::cout << i << std::endl;

    return 0;
}



/*
 * 
 * 用于转换 string 为 double
 */
template <class T>
void convertFromString(T &value, const std::string &s) {
  std::stringstream ss(s);
  ss >> value;
}



