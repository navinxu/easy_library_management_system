/*================================================================
*   Copyright (C) 2018 Navin Xu. All rights reserved.
*   
*   Filename    ：strip_space.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2018年12月02日
*   Description ：
================================================================*/
#include <iostream>
#include <string>

std::string strip_space(const std::string input_str) {

    // 去除头部空格
    std::string::size_type index_begin = 0;
    for (; index_begin < input_str.size() && isspace(input_str[index_begin]); index_begin ++);

    // 此处只有两种情况，1.如果全为空格，2.如果字符串中还有其他字符
    if (index_begin == input_str.size())
        return "";

    // 去除尾部空格
    decltype(input_str.size()) index_end;
    for (index_end = input_str.size() - 1; isspace(input_str[index_end]); index_end --);

    // 获取中间的子字符串
    decltype(input_str.size()) index;
    std::string output_str = "";
    for (index = index_begin; index <= index_end; index ++)
        output_str += input_str[index];

    return output_str;
}


int main() {

    std::string s("           Hello        World!!   ");
    std::cout << "原字符串：" << s << std::endl;
    std::cout << "去除空格后的字符串：" << strip_space(s)  << std::endl;

    std::string s2("Hello        World!!   ");
    std::cout << "原字符串：" << s2 << std::endl;
    std::cout << "去除空格后的字符串：" << strip_space(s2)  << std::endl;

    std::string s3("Hel             lo        World!!");
    std::cout << "原字符串：" << s3 << std::endl;
    std::cout << "去除空格后的字符串：" << strip_space(s3)  << std::endl;

    return 0;
}
