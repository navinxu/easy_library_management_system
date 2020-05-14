/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-read_file_from_end_to_beggining.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月28日
*   Description ：
================================================================*/
#include <iostream> // cout, endl, exit
#include <fstream> // ifstream, streampos
#include <string> // string 


/*
 *
 * 获取文件读取流
 *
 * (修改)
 */
std::ifstream get_file_read_handler(const char* filename, const char mode = 'D') {

    std::ifstream fin;
    switch (mode) {
        case 'E':
            fin.open(filename, std::ios::ate);
            break;
        default:
            // 找不到文件就创建文件
            fin.open(filename, std::ios_base::app);
            break;
    }

    if (!fin) {
        std::cout << "文件打开失败！" << std::endl;
        std::exit(1);
    }
    return fin;
}

int main() {

    std::ifstream fin = get_file_read_handler("./books.txt", 'E');
    std::streampos size = fin.tellg();
    std::cout << size << std::endl;

    // 全文件内容倒数第 3 个字符，
    // 正文倒数第 2 个字符
    fin.seekg(-1183, std::ios::end);

    char c;
    fin.get(c);
    std::cout << c << std::endl;

    //std::string line;
    //std::getline(fin, line);
    //std::cout << line << std::endl;

    return 0;
}
