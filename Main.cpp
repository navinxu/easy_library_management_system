/*================================================================
 *   Copyright (C) 2018 Navin Xu. All rights reserved.
 *
 *   文件名称：Main.cpp
 *   创 建 者：Navin Xu
 *   电子邮箱：admin@navinxu.com
 *   创建日期：2018年08月17日
 *   描    述：C++版简易图书管理系统
 ================================================================*/
/**
 *
 * # C++版图书管理系统需求设计
 * ## 角色
 *  1. 管理员
 *  2. 读者
 * ## 功能
 * ### 管理员首页
 *  管理员首页有图书信息管理、图书借还管理、用户管理等功能，其中图书信息管理有增、删、改、查功能，图书借还功能有借出图书列表（当前数目、借出日期时间、按isbn、按日期时间、借出数目排序，或者按读者列出）、借还记录（按读者查询、按图书查询）、图书预约（最多预约10本图书），用户管理功能有增加用户、删除用户、修改用户信息、查询用户等功能。
 * ### 图书信息管理
 * 1. 增
 *  只有管理员才有增加图书的权限
 *  * 若在库中找到相同的输入 ISBN 号，提示是否更新图书总库存数目(自增1)
 *  * 若在库中没有找到相同的 ISBN 号，则直接提示输入其余的图书信息项。
 * 2. 删
 *  * 根据输入的图书编号从列表中删除
 * 3. 改
 * 4. 查
 *  * 根据“ISBN”、“图书名”、“作者”、“出版社”、“图书种类”查询，支持模糊搜索，并且分页
 *  * 其中，所有条件都可选择模糊匹配，或者完全匹配
 *  * 搜寻结果可根据以上所列的查询条件进行排序，顺序或倒序，也可以按：价格、总库存量、现库存量、预约量、是否可借、最近一次借出时间、最近一次归还时间，进行排序
 *
 * ### 图书借还管理
 * 占位符
 *
 *
 */
#include <iostream> // cout
#include <string> // string
#include <fstream> // ifstream,ofstream
#include <cstring> // strtok,strcpy
#include <sstream> // stringstream
#include <iomanip>
#include <fnmatch.h> // fnmatch
#include <cctype> // isspace

int select_num = 0;
char select_char = 'C';

struct reader
{
    unsigned int reader_id;//读者编号
    std::string reader_name;//读者姓名
    std::string password; //用户密码
    unsigned int borrow_amount;//你已借阅多少本书,默认为0
    unsigned int none_return_amount;//现在还有多少本书未还，默认为0
    std::string last_borrow_date;//上次借阅时间，默认为0000.00.00
    std::string borrow[10];//你所借书的的编号，最多10本
    bool is_admin = false; //是否管理员，默认不是
    reader * next = nullptr;
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

/**
 *
 * 排序方法
 */
enum sort_order {
    ASC,
    DESC
};

void display_header();
int display_home();
int display_admin_home();
int display_logon_reader();
int add_book();
char ensure_book_info(book *);
void save_book(book *, bool);
std::ifstream get_file_read_handler(const char *, const char);
std::ofstream get_file_write_handler(const char *, const char);
void combine_book_data(book * &);
int get_max_book_id(book *);
char * string_to_char(std::string);
book * generate_book_linked_list();
void print_book(book *&);
bool book_exists(std::string);
void save_all_books(book *&);
void modify_and_save_book_info(std::string);
template <class T>
void convertFromString(T &value, const std::string &s);
void display_search_book();
int display_search_book_select();
int search_book_work(book * pBookHead, check_condition condition, std::string keyword);
void search_book(check_condition condition, bool &switch_search_book_home, bool &search_book);
void delete_book_by_id(unsigned long long book_id);
void do_delete_book_by_id(book * &pBookHead, unsigned long long book_id);
std::string strip_space_begin_end(std::string input_str);
void exchange_data_for_book(book *&p, book *&q);
short int locale_chinese_string_compare (const std::string& s1, const std::string& s2);
void sort_books(book *&pBookHead, sort_condition sort_by, sort_order order_by);
std::string cstr_to_string(const char* cstr);
unsigned int cstr_to_unsigned_int(const char* cstr);


int main() {

    // 管理首页开关
    bool switch_admin_home = true;
    // 图书按条件搜索内循环开关
    bool switch_search_book = true;
    // 搜索图书入口开关
    bool switch_search_book_home = true;
    std::string keyword = "";

    switch (display_home()) {
        case 100:
            // enter management system
            //std::cout << "管理员登录成功" << std::endl;
            while (switch_admin_home) {
                switch_admin_home = false;
                switch (display_admin_home()) {
                    case 2:
                        // 增加图书
                        switch (add_book()) {
                            case -1:
                                switch_admin_home = true;
                                break;
                            case 0:
                                return 0;
                            default:
                                switch_admin_home = true;
                                break;
                        }
                        break;
                    case 3:
                        // 查询图书
                        display_search_book();
                        switch_search_book_home = true;
                        while (switch_search_book_home) {
                            switch_search_book_home = false;
                            switch (display_search_book_select()) {
                                case 200:
                                    // 所有图书
                                    std::getchar();
                                    search_book(ALL, switch_search_book_home, switch_search_book);
                                    switch_search_book_home = true;
                                    break;
                                case 201:
                                    // 按 ISBN
                                    std::getchar();
                                    switch_search_book = true;
                                    while (switch_search_book) {
                                        switch_search_book = false;
                                        std::cout << "请输入要查询的ISBN(输入-1重新选择):";
                                        //std::cin >> keyword;
                                        search_book(ISBN, switch_search_book_home, switch_search_book);
                                    }
                                    break;
                                case 202:
                                    // 按图书名
                                    std::getchar();
                                    switch_search_book = true;
                                    while (switch_search_book) {
                                        switch_search_book = false;
                                        std::cout << "请输入要查询的图书名(输入-1重新选择):";
                                        //std::cin >> keyword;
                                        search_book(NAME, switch_search_book_home, switch_search_book);
                                    }
                                    break;
                                case 203:
                                    // 按作者
                                    std::getchar();
                                    switch_search_book = true;
                                    while (switch_search_book) {
                                        switch_search_book = false;
                                        std::cout << "请输入要查询的作者名(多个作者用 / 分割，输入-1重新选择):";
                                        //std::cin >> keyword;
                                        search_book(AUTHOR, switch_search_book_home, switch_search_book);
                                    }
                                    break;
                                case 204:
                                    // 按出版社
                                    std::getchar();
                                    switch_search_book = true;
                                    while (switch_search_book) {
                                        switch_search_book = false;
                                        std::cout << "请输入要查询的出版社(输入-1重新选择):";
                                        //std::cin >> keyword;
                                        search_book(PUBLISHER, switch_search_book_home, switch_search_book);
                                    }
                                    break;
                                case 205:
                                    // 按图书种类
                                    std::getchar();
                                    switch_search_book = true;
                                    while (switch_search_book) {
                                        switch_search_book = false;
                                        std::cout << "请输入要查询的图书类别(输入-1重新选择):";
                                        //std::cin >> keyword;
                                        search_book(CATEGORY, switch_search_book_home, switch_search_book);
                                    }
                                    break;
                                case 0:
                                    return 0;
                                case 1:
                                    switch_admin_home = true;
                                    break;
                                default:
                                    switch_search_book_home = true;
                            }

                        }
                        break;
                    case 6:
                        // 当前借出列表
                        break;
                    case 7:
                        // 借还记录
                        break;
                    case 8:
                        // 图书预约
                        break;
                    case 9:
                        // 借出图书
                        break;
                    case 11:
                        // 增加用户
                        break;
                    case 12:
                        // 查询用户
                        break;
                    case 0:
                        return 0;
                    default:
                        switch_admin_home = true;
                }
            }
            break;
        case 101:
            // enter reader system
            //std::cout << "读者登录成功" << std::endl;
            display_logon_reader();
            break;
        case 0:
            // exit system
            return 0;
        default:
            break;
    }

    return 0;
}

void display_header() {

    std::cout << "============================================================================================" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "+                                     C++ 版图书管理系统                                   +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "============================================================================================" << std::endl;
}

int display_home() {

    display_header();
    std::cout << "+                                        用户登录                                          +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;

    std::cout << "+ 请输入读者 ID（输入 0 退出系统）：";
    int reader_id = 0;
    std::cin >> reader_id;

    if (reader_id == 0) {
        return 0;
    }

    std::cout << std::endl;
    std::cout << "+ 请输入密码：";
    std::string password = "";
    std::cin >> password;

    if (reader_id == 1 && password == "1") {
        return 100;
    } else if (reader_id == 2 && password == "2") {
        return 101;
    }

    return 0;

}

int display_admin_home() {

    display_header();
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "+                                        登录成功!                                         +" << std::endl;
    std::cout << "+                                     欢迎进入管理首页                                     +" << std::endl;
    std::cout << "+                                     +++++++++++++++                                      +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "+                   图书信息管理       图书借还管理         用户管理                       +" << std::endl;
    std::cout << "+                   2. 增加图书        6. 当前借出列表      11. 增加用户                   +" << std::endl;
    std::cout << "+                   3. 查询图书        7. 借还记录          12. 查询用户                   +" << std::endl;
    std::cout << "+                     |- 图书列表        |- 借还列表          |- 用户列表                  +" << std::endl;
    std::cout << "+                     |- 按条件搜索      |- 按读者查询        |- 搜索用户                  +" << std::endl;
    std::cout << "+                     |- 修改图书信息  8. 图书预约            |- 按图书查                  +" << std::endl;
    std::cout << "+                     |- 删除图书      9. 借出图书            |- 删除用户                  +" << std::endl;
    std::cout << "+                                                             |- 修改用户                  +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cout << "+ 0:退出程序                                                                               +" << std::endl;
    std::cout << "+ 请选择: ";
    std::cin >> select_num;
    return select_num;
}

int display_logon_reader() {
    display_header();
    std::cout << "+                                       登录成功!                                          +" << std::endl;
    std::cout << "+                                    欢迎进入读者首页                                      +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    return 1;
}

int add_book() {

    display_header();
    std::cout << "+                                        添加图书                                          +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;

    bool switch_new = true;
    bool switch_add = true;
    bool is_break = false;
    while (switch_new) {

        switch_new = false;

        book * new_book = new book;

        std::getchar();
        switch_add = true;
        while (switch_add) {
            switch_add = false;

            std::cout << "ISBN(0:退出程序，-1:返回管理首页, Ctrl c 结束程序)：";
            //std::cin >> new_book->isbn;
            std::getline(std::cin, new_book->isbn);

            new_book->isbn = strip_space_begin_end(new_book->isbn);

            if (!new_book->isbn.compare("0")) {
                return 0; // 如果输入 0 ，退出程序
            } else if (!new_book->isbn.compare("-1")) {
                return -1; // 输入1 ，返回管理首页
            }

            if (book_exists(new_book->isbn)) {
                // 存在
                bool switch_is_update = true;
                while (switch_is_update) {
                    switch_is_update = false;
                    std::cout << "该图书已经存在，是否继续保存(库存增加1)(Y/N)：";
                    std::cin >> select_char;
                    switch (std::toupper(select_char)) {
                        case 'Y':
                            modify_and_save_book_info(new_book->isbn);

                            break;
                        case 'N':
                            break;
                        default:
                            std::cout << "您在说什么？我听不明白！" << std::endl;
                            std::cout << "请重新输入！" << std::endl;
                            switch_is_update = true;
                    }
                    std::cout << "=======================" << std::endl;

                }
                switch_new = true;
                // 用于退出外部循环
                is_break = true;
                break;
            } else {
                // 库中没有该图书
                // 沉默是金
                is_break = false;
            }

            // 若没有输入任何东西，则重新输入
            if (!new_book->isbn.compare("")){
                is_break = false;
                switch_add = true;
                continue;
            }
        }

        if (is_break)
            continue;

        switch_add = true;
        while (switch_add) {
            switch_add = false;

            std::cout << "图书名：";
            //std::cin >>  new_book->book_name;
            //std::getline(std::cin, new_book->book_name, '#');
            std::getline(std::cin, new_book->book_name);

            new_book->book_name = strip_space_begin_end(new_book->book_name);

            if (!new_book->book_name.compare("")){
                switch_add = true;
            }
        }

        switch_add = true;
        while (switch_add) {
            switch_add = false;

            std::cout << "作者：";
            std::getline(std::cin, new_book->author);

            new_book->author = strip_space_begin_end(new_book->author);

            if (!new_book->author.compare("")) {
                switch_add = true;
            }
        }

        switch_add = true;
        while (switch_add) {
            switch_add = false;
            std::cout << "图书类别：";
            //std::cin >> new_book->book_category;
            std::getline(std::cin, new_book->book_category);

            new_book->book_category = strip_space_begin_end(new_book->book_category);

            if (!new_book->book_category.compare(""))
                switch_add = true;

        }

        switch_add = true;
        while (switch_add) {
            switch_add = false;
            std::cout << "出版社：";
            //std::cin >> new_book->book_publisher;
            std::getline(std::cin, new_book->book_publisher);

            new_book->book_publisher = strip_space_begin_end(new_book->book_publisher);
            if (!new_book->book_publisher.compare(""))
                switch_add = true;
        }

        switch_add = true;
        std::string price;
        while (switch_add) {
            switch_add = false;
            //std::cout << "图书价格请务必输入数字，否则将可能出现无法意料的故障！" << std::endl;
            std::cout << "图书价格：";
            std::getline(std::cin, price);

            price = strip_space_begin_end(price);

            if (!price.compare(""))
                switch_add = true;
        }

        convertFromString(new_book->price, price);


        std::cout << "=======================" << std::endl;

        bool switch_is_to_save = true;

        while (switch_is_to_save) {
            switch_is_to_save = false;
            switch (std::toupper(ensure_book_info(new_book))) {
                case 'Y':
                    std::cout << "=======================" << std::endl;
                    combine_book_data(new_book);
                    //print_book(new_book);
                    save_book(new_book, true);
                    break;
                case 'N':
                    break;
                default:
                    switch_is_to_save = true;
                    std::cout << "您在说什么？我听不明白！" << std::endl;
                    std::cout << "请重新输入！" << std::endl;
                    break;
            }

            std::cout << "==============================" << std::endl;
        }
        switch_new = true;
    }
    return 0;
}

char ensure_book_info(book * pBookHead) {

    std::cout << "请确定录入以下的信息：" << std::endl;
    std::cout << "ISBN：" << pBookHead->isbn << std::endl;
    std::cout << "图书名：" << pBookHead->book_name << std::endl;
    std::cout << "作者：" << pBookHead->author << std::endl;
    std::cout << "图书类别：" << pBookHead->book_category << std::endl;
    std::cout << "出版社：" << pBookHead->book_publisher << std::endl;
    std::cout << "价格：" << pBookHead->price << std::endl;

    std::cout << "确定保存(Y/N)：";
    char ensure = 'N';
    std::cin >> ensure;

    return ensure;
}

/**
 *
 * 保存图书信息
 */

void save_book(book * pBookHead, bool single_book = true) {

    std::ofstream fout = get_file_write_handler("books.txt", 'A');

    fout << pBookHead->book_id << "|||" << pBookHead->isbn << "|||" << pBookHead->book_name  << "|||" << pBookHead->author << "|||" << pBookHead->book_publisher << "|||" <<  pBookHead->book_category << "|||" << pBookHead->price << "|||" << pBookHead->borrow_count << "|||" << pBookHead->book_amount << "|||" << pBookHead->book_current_amount << "|||" << pBookHead->appointment << "|||" << pBookHead->book_status << "|||" << pBookHead->last_borrow_date << "|||" << pBookHead->last_return_date << std::endl;

    fout.close();

    if (single_book) {
        std::cout << "保存成功！" << std::endl;
    }
}

/**
 *
 * 组合图书信息
 * 这里的改变將影响到函数外
 */
void combine_book_data(book * &pBookHead) {
    book * pHead = generate_book_linked_list();
    pBookHead->book_id = get_max_book_id(pHead) + 1;
    pBookHead->book_amount++;
    pBookHead->book_current_amount++;
    pBookHead->book_status = 1;
}

/**
 *
 * 获取最大图书 id，
 * 从而得到分配给新增图书 id。
 */
int get_max_book_id(book * pHead) {

    if (pHead == nullptr) {
        return 0;
    }

    book * pTmp = nullptr;
    while (pHead != nullptr) {

        pTmp = pHead;
        pHead = pHead->next;

        if (pHead == nullptr) {
            //cout << "Last 1:" << pTmp->book_id << endl;
            return pTmp->book_id;
        }
    }
     
    return 0;
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
    }

    return fout;
}

void truncate_file(const char* filename) {
    std::ofstream fout(filename, std::ios_base::trunc);
    if (!fout) {
        std::cout << "文件打开失败！" << std::endl;
    }
}


book * generate_book_linked_list() {
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

/**
 *
 * 通过 isbn 号查询书籍是否已经存在
 */
bool book_exists(std::string isbn) {

    book * pBookHead = generate_book_linked_list();
    while (pBookHead != nullptr) {
        // 相等为 0
        if (!pBookHead->isbn.compare(isbn)) {
            return true;
        }
        pBookHead = pBookHead->next;
    }

    return false;

}

/**
 *
 * 修改图书信息
 * 最后保存到文件。
 */
void modify_and_save_book_info(std::string isbn) {
    book * pBookHead = generate_book_linked_list();

    book * pCurr = pBookHead;

    while (pCurr != nullptr) {

        if (!pCurr->isbn.compare(isbn)) {
            // 找到
            // 更新一些信息
            // 然后把新的信息放回到原链表中

            pCurr->book_amount++;
            pCurr->book_current_amount++;
            pCurr->book_status = true;
            save_all_books(pBookHead);
            break;

        }

        pCurr = pCurr->next;
    }

    std::cout << "所有改动更新成功！" << std::endl;
}

/*
 *
 * 保存所有的图书信息
 *
 */
void save_all_books(book * &pHead) {
    // 清空原文件
    get_file_write_handler("books.txt", 'T').close();

    while (pHead != nullptr) {

        save_book(pHead, false);

        pHead = pHead->next;
    }
}

void print_book(book *&pBookHead) {
    book *tmp = nullptr;
    long long count = 1;
    while (pBookHead != nullptr) {
        //std::cout << pBookHead->book_id << "---" << pBookHead->book_name << std::endl;
        std::cout << count << " : " << pBookHead->book_id << "|||" << pBookHead->isbn << "|||" << pBookHead->book_name  << "|||" << pBookHead->author << "|||" << pBookHead->book_publisher << "|||" <<  pBookHead->book_category << "|||" << pBookHead->price << "|||" << pBookHead->borrow_count << "|||" << pBookHead->book_amount << "|||" << pBookHead->book_current_amount << "|||" << pBookHead->appointment << "|||" << pBookHead->book_status << "|||" << pBookHead->last_borrow_date << "|||" << pBookHead->last_return_date << std::endl;
        //std::cout << count << " : " << pCurrent->book_id << "|||" << pCurrent->isbn << "|||" << pCurrent->book_name  << "|||" << pCurrent->author << "|||" << pCurrent->book_publisher << "|||" <<  pCurrent->book_category << "|||" << pCurrent->price << "|||" << pCurrent->borrow_count << "|||" << pCurrent->book_amount << "|||" << pCurrent->book_current_amount << "|||" << pCurrent->appointment << "|||" << pCurrent->book_status << "|||" << pCurrent->last_borrow_date << "|||" << pCurrent->last_return_date << std::endl;
        //std::cout << pCurrent->book_id << "|||" << pCurrent->isbn << std::endl;
        tmp = pBookHead;
        delete tmp;
        pBookHead = pBookHead->next;
        count ++;
    }
    std::cout << "================================" << std::endl;
}


void display_search_book() {

    display_header();
    std::cout << "+                                        查询图书                                          +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
}

int display_search_book_select() {
    std::cout << "+ 可选择以下的查询条件进行查询：200:所有图书 201:ISBN   202:图书名" << std:: endl;
    std::cout << "+                               203:作者     204:出版社 205:图书种类" << std:: endl;
    std::cout << "+ 请选择查询条件 ID（输入 0 退出系统，输入 1 返回管理首页）：";
    std::cin >> select_num;

    return select_num;


}

/**
 * 按条件搜索图书
 * \param pBookHead 图书结构的指针
 * \param condition 查询条件，有 ISBN、图书名、作者、出版社、图书种类 几种条件，支持模糊搜索
 * \param keyword 查询关键词
 * \return 0 重新输入关键字 1 没有找到关键词
 */
int search_book_work(book * pBookHead, check_condition condition = ALL, std::string keyword = "") {

    keyword = strip_space_begin_end(keyword);
    if ((!keyword.compare("")) && (condition != ALL)) {
        // 关键字为空
        std::cout << "关键词为空，请重新输入。" << std::endl;
        return 0;
    }

    bool found = false;
    // 每次循环都会重置为 false
    bool every_found = false;

    // 组装模式
    keyword = "*" + keyword + "*";
    // 统计查询结果条目的总数量
    int res_count = 0;

    // 将查询结果组成新的链表
    book * pBookSearchRes = nullptr;
    book * pBookSearchResLast = nullptr;
    while (pBookHead != nullptr) {
        //std::stringstream ss_keyword("");
        //
        every_found = false;

        switch (condition) {
            case ISBN:
                // 按 ISBN 查询
                //ss_keyword << '*' << keyword << "*";
                //std::cout << ss_keyword.str() << std::endl;
                //std::cout << pBookHead->isbn << std::endl;
                if (!fnmatch(keyword.c_str(), pBookHead->isbn.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    //std::cout << "找到" << pBookHead->book_id << "    " << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                    found = true;
                    every_found = true;
                }
                break;
            case NAME:
                if (!fnmatch(keyword.c_str(), pBookHead->book_name.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    //std::cout << "找到" << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                    found = true;
                    every_found = true;
                }
                // 按图书名查询
                break;
            case AUTHOR:
                if (!fnmatch(keyword.c_str(), pBookHead->author.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    //std::cout << "找到" << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                    found = true;
                    every_found = true;
                }
                // 按作者查询
                break;
            case PUBLISHER:
                if (!fnmatch(keyword.c_str(), pBookHead->book_publisher.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    //std::cout << "找到" << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                    found = true;
                    every_found = true;
                }
                // 按出版社查询
                break;
            case CATEGORY:
                if (!fnmatch(keyword.c_str(), pBookHead->book_category.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    //std::cout << "找到" << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                    found = true;
                    every_found = true;
                }
                // 按图书种类查询
                break;
            case ALL:
            default:
                // 所有图书列表
                found = true;
                every_found = true;
                res_count ++;
                //std::cout << pBookHead->book_id << "    " << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
                break;

        }

        if (found && every_found) {
            //std::cout << "找到" << pBookHead->isbn << "   " << pBookHead->book_name <<  std::endl;
            // sort_res_linked_list(pBookSearchRes);
            //
            if (!pBookSearchRes)
                pBookSearchRes = pBookHead;
            else
                pBookSearchResLast->next = pBookHead;
            pBookSearchResLast = pBookHead;

        }

        pBookHead = pBookHead->next;

    }

    if (pBookSearchRes)
        pBookSearchResLast->next = nullptr;

    if (found) {
        // 未完成
        // 测试
        sort_books(pBookSearchRes, SORT_NAME, DESC);
        print_book(pBookSearchRes);
    }

    if (!found) {
        std::cout << "非常抱歉！没有找到哦！" << std::endl;
        return 1;
    } else {
        // 找到
        return 2;
    }
}



/*
 *
 * 读取关键词并执行搜索
 * \param check_condition 选择条件
 * \param bool 进入图书搜索首页
 * \param bool 进入图书搜索条件循环
 */
void search_book(check_condition condition, bool &switch_search_book_home, bool &search_book) {
    std::string keyword = "";

    if (condition != ALL)
        std::getline(std::cin, keyword);

    if (!keyword.compare("-1")) {
        switch_search_book_home = true;
        return;
    }


    std::string select;
    switch (search_book_work(generate_book_linked_list(), condition, keyword)) {
        case 0:
            // 关键词为空
        case 1:
            // 没有找到关键词
            search_book = true;
            break;
        case 2:
            // 找到
            std::cout << "============================================================================================" << std::endl;
            std::cout << "+ 请输入操作代码(-1 删除图书 -2 修改图书，输入0不选择):";
            std::getline(std::cin, select);

            if (!select.compare("0")) {
                // 不操作
                search_book = true;
                break;
            } else if (!select.compare("-1")) {
                // 删除操作

                std::cout << "请输入要进行删除的对象(图书编号)：";
                std::getline(std::cin, select);
                // 转换string字符串为数字
                int i = 0;
                convertFromString(i, select);
                delete_book_by_id(i);
            } else if (!select.compare("-2")) {
                // 修改操作
                // 未完成
            }

            search_book = true;
            break;
    }

}


void delete_book_by_id(unsigned long long book_id) {
    book * pHead = generate_book_linked_list();
    do_delete_book_by_id(pHead, book_id);
}

void do_delete_book_by_id(book * &pBookHead, unsigned long long id) {
    book * pCurrent = nullptr;
    book * pLast = nullptr;
    book * pNext = nullptr;

    bool found = false;

    pCurrent = pBookHead;
    while (pCurrent != nullptr) {
        if (pCurrent->book_id == id) {
            if (pCurrent == pBookHead)
                pBookHead = pCurrent->next;
            else
                pLast->next = pNext;

            std::cout << "以下是您需要删除的图书信息：" << std::endl;
            std::cout << "     图书编号：" << pCurrent->book_id << std::endl;
            std::cout << "     图书ISBN：" << pCurrent->isbn << std::endl;
            std::cout << "     图书标题：" << pCurrent->book_name << std::endl;
            std::cout << "您确定要删除具有以上信息的图书(Y/N)：";
            std::cin >> select_char;
            switch (std::tolower(select_char)) {
                case 'y':
                    delete pCurrent;
                    found = true;
                    break;
                case 'n':
                default:
                    std::cout << "您放弃了上一次关于删除图书的请求！" << std::endl;
                    break;
            }

            break;
        }

        pLast = pCurrent;

        pCurrent = pCurrent->next;
        if (pCurrent != nullptr)
            // 只要pCurrent不为空，则必须有next
            pNext = pCurrent->next;
    }

    //print_book(pBookHead);
    //
    if (found) {
        save_all_books(pBookHead);

        std::cout << "删除成功！" << std::endl;
    } else {
        std::cout << "非常抱歉！没有找到您所要删除的图书编号！" << std::endl;
    }
}

/**
 *
 * 按条件/顺序对图书链表进行排序
 */
void sort_res_linked_list(book * &pBookHead, sort_condition condition, sort_order order) {

    unsigned int num = 0;
    while (pBookHead != nullptr) {
        num ++;
        std::cout << num << "找到 " << pBookHead->book_id << "  " << pBookHead->isbn << "   " << pBookHead->book_name << "   " << pBookHead->book_status <<  std::endl;
        pBookHead = pBookHead->next;
    }

}

/**
 * 交换图书链表数据
 */
void exchange_data_for_book(book *&p, book *&q) {

    std::string tmp_str = "";
    unsigned int tmp_int = 0;
    unsigned long long tmp_long = 0;
    double tmp_double = 0;
    bool tmp_bool = true;

    tmp_long = p->book_id;
    p->book_id = q->book_id;
    q->book_id = tmp_long;

    tmp_str = p->isbn;
    p->isbn = q->isbn;
    q->isbn = tmp_str;

    tmp_long = p->borrow_count;
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

    tmp_long = p->appointment;
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
 * 将 string 本地化为按拼音比较
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

/**
 * 对图书信息进行排序
 */
void sort_books(book *&pBookHead, sort_condition sort_by, sort_order order_by) {
    
    book *p = nullptr, *q = nullptr, *tmp = nullptr;
    short int compare = 0;
    std::locale::global(std::locale("zh_CN.UTF-8"));
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
                    
                    //std::locale::global(std::locale("zh_CN.UTF-8"));
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
                    
                    //std::locale::global(std::locale("zh_CN.UTF-8"));
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
                    
                    //std::locale::global(std::locale("zh_CN.UTF-8"));
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
                    
                    //std::locale::global(std::locale("zh_CN.UTF-8"));
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
                    
                    //std::locale::global(std::locale("zh_CN.UTF-8"));
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
 * 工具函数
 * 去除字符串头尾的空格
 * @param input_str string 源字符串
 * @return 除去空格之后的字符串
 *
 */
std::string strip_space_begin_end(const std::string input_str) {

    // 去除头部空格
    std::string::size_type index_begin = 0;
    for (; index_begin < input_str.size() && isspace(input_str[index_begin]); index_begin ++);

    // 此处只有两种情况，1.如果全为空格，2.如果字符串中还有其他字符
    if (index_begin == input_str.size())
        return "";

    // 去除尾部空格
    decltype(input_str.size()) index_end;
    for (index_end = input_str.size() - 1; isspace(input_str[index_end]); index_end --);

    // 获取中间的子字符串
    decltype(input_str.size()) index;
    std::string output_str = "";
    for (index = index_begin; index <= index_end; index ++)
        output_str += input_str[index];

    return output_str;
}

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
 * 用于转换 string 为 double
 */
template <class T>
void convertFromString(T &value, const std::string &s) {
    std::stringstream ss(s);
    ss >> value;
}



