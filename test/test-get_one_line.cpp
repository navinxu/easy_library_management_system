/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-get_one_line.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月28日
*   Description ：
================================================================*/
#include <iostream> //cout, endl, exit()
#include <fstream> // ifstream, ate, app, streampos
using namespace std;

enum sort_order {
    ASC,
    DESC
};


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
            // 从文件未尾到文件头
            fin.open(filename, std::ios_base::ate);
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
 * 反转字符串
 * (增加)
 */
std::string reverse_string(const std::string s) {
    if (s.empty())
        return s;
    std::string temp(s.size(), 'a');
    decltype(temp.size()) i;
    for (i =  s.size() - 1; i > 0; i --)
        temp[s.size() - 1 - i] = s[i];
    temp[s.size() -1 - i] = s[0];
    return temp;
}

/**
 * 
 * 从文件末尾开始读取数据直到文件头
 * 每调用一次就读取一行，从第 n 行到第 1 行。
 * 可能返回空串
 */
std::string get_one_line(std::ifstream &fin, sort_order order) {
    std::string line("");

    if (order == ASC) {
        std::getline(fin, line);
        //cout << line << endl;

    } else if (order == DESC) {
        static decltype(fin.tellg()) pos = 1;
        static std::streampos size = fin.tellg();

        char c;
        std::string temp("");
        unsigned char flag = 1;
        for (; pos <= size; pos += 1) {
            fin.seekg(-pos, std::ios::end);
            fin.get(c);
            //line += c;
            if (flag) {
                flag = 0;
                continue;
            }
            if (c != '\n') {
                line += c;
                //putchar(c);
                if (pos == size) {
                    temp = line;
                    break;
                } else {
                    continue;
                }
            } 
            //else 
            //    putchar('\n');
            temp = line;
            line = "";
            break;
              
            //std::cout << c;
        }

        line = temp;
        line = reverse_string(line);
        //std::cout << "pos: " << -pos << std::endl;
        //std::cout << "Size: " << size<< std::endl;
        //std::cout << "Line: " << line << std::endl;
    }

    return line;
}

int main() {

    //std::ifstream fin = get_file_read_handler("./books.txt");
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //std::cout << get_one_line(fin, ASC) << std::endl;
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    //get_one_line(fin, ASC);
    std::ifstream fin = get_file_read_handler("./books.txt", 'E');
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    //get_one_line(fin, DESC);
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;
    std::cout << get_one_line(fin, DESC) << std::endl;

    //std::string s("Hello, World.");
    //std::cout << reverse_string(s) << std::endl;

    return 0;
}
