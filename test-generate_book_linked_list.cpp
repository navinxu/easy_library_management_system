/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-generate_book_linked_list.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月07日
*   Description ：
================================================================*/
#include <iostream> // cout, endl, exit()
#include <fstream> // ifstream
#include <cstring> // strtok(), strcmp()
#include <sstream> // std::stringstream
#include <string> // stoull()
#include <vector> // vector
#include <fnmatch.h> // fnmatch()
#include <cctype> // isspace()
#include <list> // list
using namespace std;

/**
 *
 * ADD
 * 分页时每页显示的最多条目
 *
 * 重命名为 per_page，原名 limit_num
 * 添加数字
 */
enum per_page {
    ONE = 1, // 1
    FIVE = 5, // 5
    TEN = 10, // 10
    TWENTY = 20 // 20
};

/**
 *
 * 排序方法
 */
enum sort_order {
    ASC,
    DESC
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

/**
 *
 * 根据传入的参数筛选出符合的条件的条目
 * param total_page    查询结果的总页数，是一个引用
 * param condition      查询的条件
 * param page           页码
 * param limit          每页最多有几条数据
 * param order          逆序从文件中取数据，还是顺序取数据
 * param keyword        关键词
 * （大修改）
 */
book* generate_book_linked_list(unsigned long long &total_page, check_condition condition = ALL, unsigned long long page = 1, per_page limit = FIVE, sort_order order = ASC, std::string keyword = "") {

    book* pHead = nullptr;

    strip_spaces(keyword);

    if (keyword.empty()) {
        if (condition != ALL)
            return  pHead;
    }

    unsigned long long items_per_page = limit;
    // 从 0 开始
    unsigned long long this_page_first_item_pos;
    unsigned long long this_page_last_item_pos;
    // 从 0 到 this_page_first_item_pos 需要跳过的计数
    unsigned long long skip_count = 0;

    this_page_first_item_pos = (page - 1) * items_per_page;
    this_page_last_item_pos = this_page_first_item_pos + items_per_page - 1;

    if (this_page_first_item_pos == 0) {
        // 防止此时 this_page_last_item_pos 控制的计数少一项
        this_page_last_item_pos ++;
    }

    // fnmatch() 函数任意位置匹配
    keyword = "*" + keyword + "*";

    std::ifstream fin;
    if (order == ASC)
        fin = get_file_read_handler("books.txt");
    else if(order == DESC)
        fin = get_file_read_handler("books.txt", 'E');

    std::string line;
    book *pCurrent = nullptr, *pPrev = nullptr;
    std::vector<std::string> temp;
    //std::getline(fin, line);
    line = get_one_line(fin, order);
    while (!line.empty()) {
        //std::cout << line << std::endl;
        //
        pCurrent = new book();

        temp = split(line, "|||");
        //std::getline(fin, line);
        line = get_one_line(fin, order);

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

        if (skip_count < this_page_first_item_pos) {
            skip_count ++;
            continue;
        }

        // 如果前面不加 1，就会导致计数少 1
        if (this_page_last_item_pos == this_page_first_item_pos)
            break;

        this_page_last_item_pos --;

        //cout << this_page_last_item_pos << endl;

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

/**
 * 
 * 打印链表
 * 这里的 order 指对查询结果链表进行顺序或逆序的排序
 * (修改)
 * */
void display_books(book*& pHead, sort_order order) {
    book*& pTmp = pHead;
    if (order == DESC) {
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
    } else if (order == ASC) {
        while (pTmp != nullptr) {
            std::cout << pTmp->book_id << "|||" << pTmp->isbn << "|||" << pTmp->book_name  << "|||" << pTmp->author << "|||" << pTmp->book_publisher << "|||" <<  pTmp->book_category << "|||" << pTmp->price << "|||" << pTmp->borrow_count << "|||" << pTmp->book_amount << "|||" << pTmp->book_current_amount << "|||" << pTmp->appointment << "|||" << pTmp->book_status << "|||" << pTmp->last_borrow_date << "|||" << pTmp->last_return_date << std::endl;
            delete pTmp;
            pTmp = pTmp->next;
        }
    }
}


int main() {

    //std::string s = "   我 是 中国人  ";
    //std::cout << s << std::endl;
    //strip_spaces(s);
    //std::cout << s << std::endl;
    //return 0;

    //std::string s("97871154093");
    std::string keyword("   ");
    //book* pBook = generate_book_linked_list(GLOBAL, 1, FIVE, keyword);
    unsigned long long int total_page = 0;
    book* pBook = generate_book_linked_list(total_page, ALL, 2, FIVE, DESC, keyword);
    if (pBook)
        //display_books(pBook, ASC);
        display_books(pBook, DESC);
    else 
        std::cout << "很遗憾！没有找到哦～～" << std::endl;

    return 0;
}
