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
#include <sstream> // std::stringstream
#include <string> // stoull()
#include <vector> // 

/**
 *
 * ADD
 * 分页时每页显示的最多条目
 */
enum limit_num {
    FIVE, // 5
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
 * std::string 类型转char *  类型
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

book* generate_book_linked_list(check_condition condition, std::string keyword, unsigned long long page, limit_num limit) {
    std::ifstream fin = get_file_read_handler("books.txt");

    std::string line;
    book *pHead = nullptr, *pCurrent = nullptr, *pPrev = nullptr;
    std::vector<std::string> temp;
    while (std::getline(fin, line)) {
        //std::cout << line << std::endl;
        //
        pCurrent = new book();

        temp = split(line, "|||");

        /*
    enum check_condition {
        ALL,
        ISBN,
        NAME,
        AUTHOR,
        PUBLISHER,
        CATEGORY
    };
    */
        switch ( condition ) {
            case ISBN:
                break;
            case NAME:
                break;
            case AUTHOR:
                break;
            case PUBLISHER:
                break;
            case CATEGORY:
                break;
            case ALL:
            default:
                break;
        }

        /*
        for (decltype(temp.size()) index = 0; index != temp.size(); index ++) {

            switch (index) {
                case 0:
                    //cout << "1:" << temp[index] << endl;
                    pCurrent->book_id = std::stoull(temp[index]);
                    // 不知道为何这也行
                    //pCurrent->book_id = std::stoull(temp[index]);
                    break;
                case 1:
                    //cout << "2:" << temp[index] << endl;
                    pCurrent->isbn = temp[index];
                    break;
                case 2:
                    //cout << "3:" <<temp[index] << endl;
                    pCurrent->book_name = temp[index];
                    break;
                case 3:
                    pCurrent->author = temp[index];
                    break;
                case 4:
                    pCurrent->book_publisher = temp[index];
                    break;
                case 5:
                    pCurrent->book_category = temp[index];
                    break;
                case 6:
                    pCurrent->price = std::stod(temp[index]);
                    break;
                case 7:
                    pCurrent->borrow_count = std::stoull(temp[index]);
                    break;
                case 8:
                    pCurrent->book_amount = std::stoul(temp[index]);
                    break;
                case 9:
                    pCurrent->book_current_amount = std::stoul(temp[index]);
                    break;
                case 10:
                    pCurrent->appointment = std::stoull(temp[index]);
                    break;
                case 11:
                    pCurrent->book_status = std::stoul(temp[index]);
                    break;
                case 12:
                    pCurrent->last_borrow_date = temp[index];
                    break;
                case 13:
                    pCurrent->last_return_date = temp[index];
                    break;
            }
        }
        */
        pCurrent->book_id = std::stoull(temp[0]);
        pCurrent->isbn = temp[1];
        pCurrent->book_name = temp[2];
        pCurrent->author = temp[3];
        pCurrent->book_publisher = temp[4];
        pCurrent->book_category = temp[5];
        pCurrent->price = std::stod(temp[6]);
        pCurrent->borrow_count = std::stoull(temp[7]);
        pCurrent->book_amount = std::stoul(temp[8]);
        pCurrent->book_current_amount = std::stoul(temp[9]);
        pCurrent->appointment = std::stoull(temp[10]);
        pCurrent->book_status = std::stoul(temp[11]);
        pCurrent->last_borrow_date = temp[12];
        pCurrent->last_return_date = temp[13];

        if (!pHead) {
            pHead = new book();
            pHead = pCurrent;
            pPrev = new book();
        } else {
            pPrev->next = pCurrent;
        }
        pPrev = pCurrent;
    }

    if (!pHead)
        pCurrent->next = nullptr;

    return pHead;
}

void display_books(book*& pHead) {
    book*& pTmp = pHead;
    while (pTmp) {
        std::cout << pTmp->book_id << "|||" << pTmp->isbn << "|||" << pTmp->book_name  << "|||" << pTmp->author << "|||" << pTmp->book_publisher << "|||" <<  pTmp->book_category << "|||" << pTmp->price << "|||" << pTmp->borrow_count << "|||" << pTmp->book_amount << "|||" << pTmp->book_current_amount << "|||" << pTmp->appointment << "|||" << pTmp->book_status << "|||" << pTmp->last_borrow_date << "|||" << pTmp->last_return_date << std::endl;
        delete pTmp;
        pTmp = pTmp->next;
    }
}


int main() {

    book* pBook = generate_book_linked_list(ISBN, "大型网站", 1, FIVE);
    display_books(pBook);

    return 0;
}
