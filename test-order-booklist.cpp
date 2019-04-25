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
#include <locale> 

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
 * 排序方式
 */
enum sort_order {
    ASC, // 顺序
    DESC // 倒序
};

/**
 * 排序条件
 */
enum sort_condition {
    SORT_ID,
    SORT_ISBN,
    SORT_NAME,
    SORT_AUTHOR,
    SORT_PUBLISHER,
    SORT_CATEGORY,
    SORT_PRICE,
    SORT_AMOUNT,
    SORT_CURRENT_AMOUNT,
    SORT_APPOINTMENT,
    SORT_STATUS,
    SORT_LAST_BORROW_DATE,
    SORT_LAST_RETURN_DATE
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

/**
 * param begin,unsigned long,the posision beginning
 * param limit,unsigned long, the number of books fetch once
 */
//book * generate_book_linked_list(unsigned long begin_pos, unsigned long limit) {
book * generate_book_linked_list() {
    std::ifstream fin = get_file_read_handler("test.txt");

    std::string _line;
    char* delim = string_to_char("|||");
    char* tmp = nullptr;
    char* line = nullptr;
    unsigned short count = 0;
    book * pHead = nullptr;
    book * pCurrent = nullptr;
    book * pLast = nullptr;

    //unsigned long position = 0, pos_count = 0;

    while (!fin.eof()) {

        /*
        if (position || limit) {
            if (position < begin_pos) {
                position ++;
                continue;
            }

            if (pos_count > limit)
                break;

            position ++;

            if (position >= begin_pos)
                pos_count ++;
        }
        */

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
                    //pCurrent->book_status = (bool)tmp;
                    if (!std::strcmp(tmp, "0"))
                        pCurrent->book_status = false;
                    else
                        pCurrent->book_status = true;

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

void print_book(book *pBookHead) {
    book *&pCurrent = pBookHead;
    long long count = 1;
    while (pCurrent != nullptr) {
        //std::cout << pBookHead->book_id << "---" << pBookHead->book_name << std::endl;
        //std::cout << count << " : " << pBookHead->book_id << "|||" << pBookHead->isbn << "|||" << pBookHead->book_name  << "|||" << pBookHead->author << "|||" << pBookHead->book_publisher << "|||" <<  pBookHead->book_category << "|||" << pBookHead->price << "|||" << pBookHead->borrow_count << "|||" << pBookHead->book_amount << "|||" << pBookHead->book_current_amount << "|||" << pBookHead->appointment << "|||" << pBookHead->book_status << "|||" << pBookHead->last_borrow_date << "|||" << pBookHead->last_return_date << std::endl;
        std::cout << count << " : " << pCurrent->book_id << "|||" << pCurrent->isbn << "|||" << pCurrent->book_name  << "|||" << pCurrent->author << "|||" << pCurrent->book_publisher << "|||" <<  pCurrent->book_category << "|||" << pCurrent->price << "|||" << pCurrent->borrow_count << "|||" << pCurrent->book_amount << "|||" << pCurrent->book_current_amount << "|||" << pCurrent->appointment << "|||" << pCurrent->book_status << "|||" << pCurrent->last_borrow_date << "|||" << pCurrent->last_return_date << std::endl;
        //std::cout << pCurrent->book_id << "|||" << pCurrent->isbn << std::endl;
        //tmp = pBookHead;
        //delete tmp;
        //pBookHead = pBookHead->next;
        pCurrent = pCurrent->next;
        count ++;
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

/**
 *
 * s1 > s2 ret 1
 * s1 < s2 ret -1
 * s1 = s2 ret 0
 * 小写字母 < 大写字母
 * 数字：55 < 6
 */
short int locale_chinese_string_compare (const std::string& s1, const std::string& s2) {

   const std::collate<char>& col = 
       std::use_facet<std::collate<char> >(std::locale()); // Use the global locale

   const char* pb1 = s1.data();
   const char* pb2 = s2.data();

   //return (col.compare(pb1, pb1 + s1.size(),
   //                    pb2, pb2 + s2.size()) < 0);
   return col.compare(pb1, pb1 + s1.size(),
                       pb2, pb2 + s2.size());
}

void sort_books(book *&pBookHead, sort_condition sort_by, sort_order order_by) {
    
    book *p = nullptr, *q = nullptr, *tmp = nullptr;
    short int compare = 0;
    // 应用选择法对数据进行排序
    for (p = pBookHead; p != nullptr; p = p->next) {
        tmp = p;
        for (q = p->next; q != nullptr; q = q->next) {
            switch (sort_by) {
                case SORT_ISBN:
                    compare = tmp->isbn.compare(q->isbn);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_ID:
                    compare = tmp->book_id > q->book_id;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_NAME:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->book_name, q->book_name);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_AUTHOR:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->author, q->author);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_PUBLISHER:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->book_publisher, q->book_publisher);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_CATEGORY:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->book_category, q->book_category);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_PRICE:
                    compare = tmp->price > q->price;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_AMOUNT:
                    compare = tmp->book_amount > q->book_amount;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_CURRENT_AMOUNT:
                    compare = tmp->book_current_amount > q->book_current_amount;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_APPOINTMENT:
                    compare = tmp->appointment > q->appointment;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_STATUS:
                    compare = tmp->book_status > q->book_status;
                    switch (order_by) {
                        case ASC:
                            if (compare)
                                tmp = q;
                            break;
                        case DESC:
                            if (!compare)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_LAST_BORROW_DATE:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->last_borrow_date, q->last_borrow_date);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
                case SORT_LAST_RETURN_DATE:
                    
                    std::locale::global(std::locale("zh_CN.UTF-8"));
                    compare = locale_chinese_string_compare(tmp->last_return_date, q->last_return_date);
                    switch (order_by) {
                        case ASC:
                            if (compare > 0)
                                tmp = q;
                            break;
                        case DESC:
                            if (compare < 0)
                                tmp = q;
                            break;
                    }
                    break;
            }
        }

        //continue;
        if (tmp != p) {
           // std::cout << p->isbn << " : " << tmp->isbn << std::endl;
            exchange_data_for_book(p, tmp);
            //std::cout << p->isbn << " = " << tmp->isbn << std::endl;

        }
    }
}

/**
 * 
 *
 */

int main() {

    book * pBook = generate_book_linked_list();
    print_book(pBook);
    sort_books(pBook, SORT_NAME, DESC);
    print_book(pBook);
    //sort_books(pBook);

    return 0;
}
