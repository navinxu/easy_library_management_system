/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-delete_book_by_id.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年09月12日
*   Description ：
================================================================*/
#include <iostream>
#include <fstream>
#include <vector>

/**
 *
 * 排序方法
 */
enum sort_order {
    ASC,
    DESC
};

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
 * 反转字符串
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
 * 获得文件写入流
 */
std::ofstream  get_file_write_handler(const char * filename, const char mode = 'D') {
    std::ofstream fout;
    switch (mode) {
        case 'A':
            // 追加内容到文件末尾
            fout.open(filename, std::ios_base::app);
            break;
        case 'T':
            // 打开文件之前清空文件
            fout.open(filename, std::ios_base::trunc);
            break;
        default:
            // 文件不存在就新建空文件
            // 重新写入文件（抹去＋写入）
            fout.open(filename);
    }
    if (!fout) {
        std::cout << "文件打开失败！" << std::endl;
        std::exit(0);
    }

    return fout;
}

/**
 * 
 * 从文件末尾开始读取数据直到文件头
 * 每调用一次就读取一行，从第 n 行到第 1 行。
 * 可能返回空串
 * (增加)
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

/**
 *
 * 分割字符串
 */
std::vector<std::string> split(std::string& input, const std::string& delimiter) {
    std::vector<std::string> result;

    if (input.empty())
        return result;

    // 在后面加上分隔符以获取最后面一部分的字符串
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
 * 通过书籍 ID 删除书籍
 * （新增）
 */
void delete_book_by_id(unsigned long long int book_id) {
    std::ifstream fin;
    std::ofstream fout;

    fin = get_file_read_handler("./books.txt", 'A');
    fout = get_file_write_handler("./books.tmp", 'T');

    fin.get();
    if (fin.eof())
        return;

    fin.seekg(0);

    std::string line;

    line = get_one_line(fin, ASC);

    std::string line2 = line;

    std::vector<std::string> temp;
    while (!line.empty()) {
        temp = split(line, "|||");
        //std::cout << temp[0] << std::endl;
        if (std::stoull(temp[0]) != book_id)
            fout << line2 << std::endl;
        line = get_one_line(fin, ASC);
        line2 = line;
    }

    fout.close();
    fin.close();

    fout = get_file_write_handler("./books.txt", 'T');
    fin = get_file_read_handler("./books.tmp", 'A');

    line = get_one_line(fin, ASC);
    while (!line.empty()) {
        fout << line << std::endl;
        line = get_one_line(fin, ASC);
    }
    fin.close();
    fout.close();

    fout = get_file_write_handler("./books.tmp", 'T');
    fout.close();

}



int main() {

    delete_book_by_id(1);

    return 0;
}
