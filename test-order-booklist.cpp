/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-order-booklist.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年03月31日
*   Description ：
================================================================*/
#include <iostream> // cout
#include <fstream> // ifstream
#include <cstring> // strtok, strcpy
#include <string> // string

struct book
{
    unsigned long book_id;
    std::string isbn;//图书检索号
    unsigned int borrow_count = 0;//图书借阅量,初始化为0
    std::string book_publisher; //出版社
    std::string book_name;//书名
    std::string book_category;//图书种类
    double price = 0.0;//图书价格
    unsigned int book_amount = 0;//图书总库存存量
    unsigned int book_current_amount = 0;//图书现库存量
    std::string author;//图书作者
    unsigned int appointment = 0;//图书预约量,初始化为0
    bool book_status = false;//是否可借,初始为不可以
    std::string last_borrow_date = "0000-00-00";//图书最近一次借出时间，默认为0000-00-00；
    std::string last_return_date = "0000-00-00";//图书最近一次归还时间，默认为0000-00-00；
    book * next = nullptr;
};


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

/**
 *
 * string 类型转char *  类型
 */
char * string_to_char(std::string str) {
    char *c = new char[str.length() + 1];

    std::strcpy(c, str.c_str());
    return c;
}

book * generate_book_link_table() {
    std::ifstream fin = get_file_read_handler("test.txt");

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
                    pCurrent->book_id = std::atoi(tmp);
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
                    pCurrent->borrow_count = std::atoi(tmp);
                    break;
                case 9:
                    pCurrent->book_amount = std::atoi(tmp);
                    break;
                case 10:
                    pCurrent->book_current_amount = std::atoi(tmp);
                    break;
                case 11:
                    pCurrent->appointment = std::atoi(tmp);
                    break;
                case 12:
                    pCurrent->book_status = (bool)tmp;
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

void print_book(book * pBookHead) {
    book * pCurrent = pBookHead;
    while (pCurrent != nullptr) {
        //std::cout << pBookHead->book_id << "---" << pBookHead->book_name << std::endl;
        //std::cout << pBookHead->book_id << "|||" << pBookHead->isbn << "|||" << pBookHead->book_name  << "|||" << pBookHead->author << "|||" << pBookHead->book_publisher << "|||" <<  pBookHead->book_category << "|||" << pBookHead->price << "|||" << pBookHead->borrow_count << "|||" << pBookHead->book_amount << "|||" << pBookHead->book_current_amount << "|||" << pBookHead->appointment << "|||" << pBookHead->book_status << "|||" << pBookHead->last_borrow_date << "|||" << pBookHead->last_return_date << std::endl;
        std::cout << pCurrent->book_id << "|||" << pCurrent->isbn << "|||" << pCurrent->book_name  << "|||" << pCurrent->author << "|||" << pCurrent->book_publisher << "|||" <<  pCurrent->book_category << "|||" << pCurrent->price << "|||" << pCurrent->borrow_count << "|||" << pCurrent->book_amount << "|||" << pCurrent->book_current_amount << "|||" << pCurrent->appointment << "|||" << pCurrent->book_status << "|||" << pCurrent->last_borrow_date << "|||" << pCurrent->last_return_date << std::endl;
        //std::cout << pCurrent->book_id << "|||" << pCurrent->isbn << std::endl;
        pCurrent = pCurrent->next;
    }
    std::cout << "================================" << std::endl;
}

void exchange_data_for_book(book *&p, book *&q) {

    std::string tmp_str = "";
    unsigned int tmp_int = 0;
    unsigned long tmp_long = 0;
    double tmp_double = 0;
    bool tmp_bool = true;

    tmp_long = p->book_id;
    p->book_id = q->book_id;
    q->book_id = tmp_long;

    tmp_str = p->isbn;
    p->isbn = q->isbn;
    q->isbn = tmp_str;

    tmp_int = p->borrow_count;
    p->borrow_count = q->borrow_count;
    q->borrow_count = tmp_int;

    tmp_str = p->book_publisher;
    p->book_publisher = q->book_publisher;
    q->book_publisher = tmp_str;

    tmp_str = p->book_name;
    p->book_name = q->book_name;
    q->book_name = tmp_str;

    tmp_str = p->book_category;
    p->book_category = q->book_category;
    q->book_category = tmp_str;

    tmp_double = p->price;
    p->price = q->price;
    q->price = tmp_double;

    tmp_int = p->book_amount;
    p->book_amount = q->book_amount;
    q->book_amount = tmp_int;

    tmp_int = p->book_current_amount;
    p->book_current_amount = q->book_current_amount;
    q->book_current_amount = tmp_int;

    tmp_str = p->author;
    p->author = q->author;
    q->author = tmp_str;

    tmp_int = p->appointment;
    p->appointment = q->appointment;
    q->appointment = tmp_int;

    tmp_bool = p->book_status;
    p->book_status = q->book_status;
    q->book_status = tmp_bool;

    tmp_str = p->last_borrow_date;
    p->last_borrow_date = q->last_borrow_date;
    q->last_borrow_date = tmp_str;

    tmp_str = p->last_return_date;
    p->last_return_date = q->last_return_date;
    q->last_return_date = tmp_str;




}

book* order_by_isbn(book *pBookHead) {
    
    book * p = nullptr, *q = nullptr;
    std::string right_str = "";
    //std::string tmp = "";
    for (p = pBookHead; (p != nullptr) && (p->isbn != right_str); p = p->next) {
        for (q = p->next; q != nullptr; q = q->next) {
            if (p->isbn.compare(q->isbn) > 0) {
                /*
                tmp = p->isbn;
                p->isbn = q->isbn;
                q->isbn = tmp;
                */
                exchange_data_for_book(p, q);
                right_str = p->isbn;
            }
        }
    }
    
    return pBookHead;
}

/**
 * 
 *
 */

int main() {

    book * pBook = generate_book_link_table();
    print_book(pBook);
    print_book(order_by_isbn(pBook));

    return 0;
}
