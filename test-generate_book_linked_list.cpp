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
#include <vector> // vector
#include <fnmatch.h> // fnmatch()
#include <cctype> // isspace()

/**
 *
 * ADD
 * 分页时每页显示的最多条目
 *
 * 重命名为 per_page，原名 limit_num
 * 添加数字
 */
enum per_page {
    FIVE = 5, // 5
    TEN = 10, // 10
    TWENTY = 20 // 20
};

// 添加 prev 属性
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
    book * prev = nullptr;
};

/*
 * 图书检索条件
 *
 * 添加 GLOBAL
 */
enum check_condition {
    ALL,
    GLOBAL,
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

/**
 *
 * 添加
 * 去除字符串两头的空格
 * 结果有可能是空字符串
 */
void strip_spaces(std::string& s) {
    std::string temp;
    decltype(s.size()) head, tail;
    for (head = 0; head < s.size() && isspace(s[head]); head ++)
        ;

    if (head == s.size())
        temp = "";
    else {

        for (tail = s.size() - 1; tail > head && isspace(s[tail]); tail --)
            ;
        tail ++;

        for (;head < tail; head ++)
            temp += s[head];
    }
    s = temp;
}

book* generate_book_linked_list(check_condition condition, unsigned long long page, per_page limit, std::string keyword = "") {

    book* pHead = nullptr;

    strip_spaces(keyword);

    if (keyword.empty()) {
        if (condition != ALL)
            return  pHead;
    }

    // fnmatch() 函数任意位置匹配
    keyword = "*" + keyword + "*";

    std::ifstream fin = get_file_read_handler("books.txt");

    std::string line;
    book *pCurrent = nullptr, *pPrev = nullptr;
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
        // 假设没匹配到
        bool flag = false;
        switch ( condition ) {
            case ISBN:
                // 返回 0 为匹配到
                if (!fnmatch(keyword.c_str(), temp[1].c_str(), FNM_NOESCAPE | FNM_CASEFOLD))
                    flag = true;
                break;
            case NAME:
                if (!fnmatch(keyword.c_str(), temp[2].c_str(), FNM_NOESCAPE | FNM_CASEFOLD))
                    flag = true;
                break;
            case AUTHOR:
                if (!fnmatch(keyword.c_str(), temp[3].c_str(), FNM_NOESCAPE | FNM_CASEFOLD))
                    flag = true;
                break;
            case PUBLISHER:
                if (!fnmatch(keyword.c_str(), temp[4].c_str(), FNM_NOESCAPE | FNM_CASEFOLD))
                    flag = true;
                break;
            case CATEGORY:
                if (!fnmatch(keyword.c_str(), temp[5].c_str(), FNM_NOESCAPE | FNM_CASEFOLD))
                    flag = true;
                break;
            case GLOBAL:
                for (unsigned char index = 1; index <= 5; index ++)
                    // 只要匹配任何一个，就退出这层循环
                    if (!fnmatch(keyword.c_str(), temp[index].c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                        flag = true;
                        break;
                    }
                break;
            case ALL:
            default:
                flag = true;
                break;
        }

        // 如果没有匹配到，那么不再进行以下操作
        if (!flag)
            continue;

        /*
        for (decltype(temp.size()) index = 0; index != temp.size(); index ++) {

            switch (index) {
                case 0:
                    //cout << "1:" << temp[index] << endl;
                    pCurrent->book_id = std::stoull(temp[index]);
    i                // 不知道为何这也行
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
            pCurrent->prev = pPrev;
        }
        pPrev = pCurrent;
    }

    if (!pHead)
        pCurrent->next = nullptr;

    return pHead;
}

void display_books(book*& pHead) {
    book*& pTmp = pHead;
    book* prev = nullptr;
    while (pTmp) {
        prev = pTmp;
        pTmp = pTmp->next;
    }

    while (prev) {
        std::cout << prev->book_id << "|||" << prev->isbn << "|||" << prev->book_name  << "|||" << prev->author << "|||" << prev->book_publisher << "|||" <<  prev->book_category << "|||" << prev->price << "|||" << prev->borrow_count << "|||" << prev->book_amount << "|||" << prev->book_current_amount << "|||" << prev->appointment << "|||" << prev->book_status << "|||" << prev->last_borrow_date << "|||" << prev->last_return_date << std::endl;
        delete prev;
        prev = prev->prev;
    }
}


int main() {

    //std::string s = "   我 是 中国人  ";
    //std::cout << s << std::endl;
    //strip_spaces(s);
    //std::cout << s << std::endl;
    //return 0;

    //std::string s("97871154093");
    std::string keyword("   计算机 ");
    //book* pBook = generate_book_linked_list(GLOBAL, 1, FIVE, keyword);
    book* pBook = generate_book_linked_list(GLOBAL, 1, FIVE, keyword);
    if (pBook)
        display_books(pBook);
    else 
        std::cout << "很遗憾！没有找到哦～～" << std::endl;

    return 0;
}
