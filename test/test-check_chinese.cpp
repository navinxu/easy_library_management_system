/*================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   
*   文件名称：test.cpp
*   创 建 者：Navin Xu
*   电子邮箱：admin@navinxu.com
*   创建日期：2018年06月04日
*   描    述：
================================================================*/
#include <iostream>
#include <string>
#include <cstring>

bool checkChinese(char[]);
char * string_to_char(std::string);

int main() {

    std::string astring = "我))";
    std::cout << astring.length() << std::endl;

    if (checkChinese(string_to_char(astring)))
        std::cout << "chinese" << std::endl;
    else
        std::cout << "not chinese" << std::endl;



    return 0;
}

bool checkChinese(char word[]){
    char c = 'a';

    c = *word;
    while (c != '\0') {

        if (c < 0 || c > 127) 
            return true;
        else 
            return false;
        word += 1;
        c = *word;
    }
}

/**
 *
 * string 类型转char *  类型
 */
char * string_to_char(std::string str) {
    char *c = new char[str.length() + 1];

    std::strcpy(c, str.c_str());
    return c;
}
