/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-generate_book_linked_list.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月07日
*   Description ：
================================================================*/
#include <iostream> // cout, endl
#include <fstream> // ifstream
#include <cstring> // strtok(), strcmp()
#include <sstream> // stringstream
#include <string> // stoull()

/**
 *
 * ADD
 * 分页时每页显示的最多条目
 */
enum limit_num {
    FiVE, // 5
    TEN, // 10
    TWENTY // 20
};

struct book
{
    unsigned long long book_id;
    std::string isbn;//图书检索号
    unsigned long long borrow_count = 0;//图书借阅量,初始化为0
    std::string book_publisher; //出版社
    std::string book_name;//书名
    std::string book_category;//图书种类
    double price = 0.0;//图书价格
    unsigned int book_amount = 0;//图书总库存存量
    unsigned int book_current_amount = 0;//图书现库存量
    std::string author;//图书作者
    unsigned long long appointment = 0;//图书预约量,初始化为0
    short int book_status = 0;//是否可借,初始为不可以
    std::string last_borrow_date = "0000-00-00";//图书最近一次借出时间，默认为0000-00-00；
    std::string last_return_date = "0000-00-00";//图书最近一次归还时间，默认为0000-00-00；
    book * next = nullptr;
};

/*
 * 图书检索条件
 */
enum check_condition {
    ALL,
    ISBN,
    NAME,
    AUTHOR,
    PUBLISHER,
    CATEGORY
};

/**
 *
 *  char 字符串转换成 string
 */
std::string cstr_to_string(const char* cstr) {
    std::string s;
    std::stringstream ss;
    ss << cstr;
    ss >> s;
    return s;
}

/**
 *
 * 将 char 类型字符串转换成 unsigned int 类型
 */
unsigned int cstr_to_unsigned_int(const char* cstr) {
    std::stringstream ss;
    unsigned int value;
    ss << cstr;
    ss >> value;
    return value;
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

/*
 *
 * 获取文件读取流
 */
std::ifstream get_file_read_handler(const char* filename, const char mode = 'D') {

    std::ifstream fin;
    switch (mode) {
        default:
            // 找不到文件就创建文件
            fin.open(filename, std::ios_base::app);
    }

    if (!fin) {
        std::cout << "文件打开失败！" << std::endl;
    }
    return fin;
}

book * generate_book_linked_list(check_condition condition, std::string keyword, unsigned long long page, limit_num limit) {
    std::ifstream fin = get_file_read_handler("books.txt");

    std::string _line;
    char* delim = string_to_char("|||");
    char* tmp = nullptr;
    char* line = nullptr;
    unsigned short count = 0;
    book * pHead = nullptr;
    book * pCurrent = nullptr;
    book * pLast = nullptr;
    while (!fin.eof()) {
        getline(fin, _line);
        //std::cout << str << endl;
        line = string_to_char(_line);
        tmp = std::strtok(line, delim);
        count = 0;

        if (tmp != nullptr)
            pCurrent = new book;

        while (tmp != nullptr) {
            //std::cout << tmp << endl;

            switch (++count) {
                case 1:
                    //cout << "1:" << tmp << endl;
                    pCurrent->book_id = std::stoull(cstr_to_string(tmp));
                    // 不知道为何这也行
                    //pCurrent->book_id = std::stoull(tmp);
                    break;
                case 2:
                    //cout << "2:" << tmp << endl;
                    pCurrent->isbn = tmp;
                    break;
                case 3:
                    //cout << "3:" <<tmp << endl;
                    pCurrent->book_name = tmp;
                    break;
                case 4:
                    pCurrent->author = tmp;
                    break;
                case 5:
                    pCurrent->book_publisher = tmp;
                    break;
                case 6:
                    pCurrent->book_category = tmp;
                    break;
                case 7:
                    pCurrent->price = std::atof(tmp);
                    break;
                case 8:
                    pCurrent->borrow_count = std::stoull(cstr_to_string(tmp));
                    break;
                case 9:
                    pCurrent->book_amount = cstr_to_unsigned_int(tmp);
                    break;
                case 10:
                    pCurrent->book_current_amount = cstr_to_unsigned_int(tmp);
                    break;
                case 11:
                    pCurrent->appointment = std::stoull(cstr_to_string(tmp));
                    break;
                case 12:
                    //pCurrent->book_status = (bool)tmp;
                    /*
                    if (!std::strcmp(tmp, "0"))
                        pCurrent->book_status = 0;
                    else
                        pCurrent->book_status = 1;
                    */
                    pCurrent->book_status = cstr_to_unsigned_int(tmp);

                    break;
                case 13:
                    pCurrent->last_borrow_date = tmp;
                    break;
                case 14:
                    pCurrent->last_return_date = tmp;
                    break;
            }
            tmp = std::strtok(nullptr, delim);
        }


        if (pHead == nullptr) {
            pHead = pCurrent;
        } else {
            pLast->next = pCurrent;
        }

        pLast = pCurrent;

    }

    /*
       if (fin.eof()) {
       cout << "file empty" << endl;
       }
       */

    fin.close();

    if (pHead != nullptr) {
        pLast->next = nullptr;
    }


    return pHead;


}


int main() {

    return 0;
}
