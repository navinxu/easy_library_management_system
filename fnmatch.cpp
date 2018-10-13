/*================================================================
*   Copyright (C) 2018 Navin Xu. All rights reserved.
*   
*   Filename    ：fnmatch.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2018年10月13日
*   Description ：
================================================================*/
#include <iostream>
#include <sstream>
#include <fnmatch.h>

int main() {

    std::string keyword = "978-7-121-";
    std::stringstream ss("");
    ss << "*" <<  keyword << "*";

    if (!fnmatch(ss.str().c_str(), "978-7-121-212", FNM_NOESCAPE | FNM_CASEFOLD)) {
        std::cout << "找到" << std::endl;
    } else {
        std::cout << "没有找到" << std::endl;
    }

    return 0;
}

