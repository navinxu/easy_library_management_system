/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-get_max_book_id.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年09月12日
*   Description ：
================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


/*
 *
 * 获取文件读取流
 */
std::ifstream get_file_read_handler(const char* filename, const char mode = 'D') {

    std::ifstream fin;
    switch (mode) {
        case 'E':
            // 从文件未尾到文件头
            fin.open(filename, std::ios_base::ate | std::ios_base::app);
            break;
        default:
            // 找不到文件就创建文件
            fin.open(filename, std::ios_base::app);
    }

    if (!fin) {
        std::cout << "文件打开失败！" << std::endl;
        std::exit(1);
    }

    return fin;
}

/**
 *
 * 分割由多个字符相隔的字符串
 */
std::vector<std::string> split(std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;

    if (input.empty())
        return result;

    // 在后面加上分隔符以获取最后面的有效字符串
    input += delimiter;
    std::string temp;
    std::string::size_type pos;

    pos = input.find(delimiter, 0);

    while (pos != temp.npos) {
        temp = input.substr(0, pos);
        result.push_back(temp);
        input = input.substr(pos + delimiter.size(), input.size());
        pos = input.find(delimiter, 0);
    }

    return result;
}

/**
 *
 * 获取最大图书 id，
 * 从而得到分配给新增图书 id。
 */
unsigned long long get_max_book_id() {

    std::ifstream fin = get_file_read_handler("./books.txt", 'D');

    std::string line, temp;
    std::getline(fin, line);
    temp = line;

    while (!fin.eof()) {
        temp = line;
        std::getline(fin, line);
    }

    //std::cout << temp << std::endl;
    if (temp.empty())
        return 0;
    else {
        std::vector<std::string> vec = split(temp, "|||");
        return std::stoull(vec[0]);
    }

      
     return 0;
 }


int main() {

    std::cout << get_max_book_id() << std::endl;

    return 0;
}
