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
#include <string> // string, stoull()
#include <fstream> // ifstream,ofstream
#include <cstring> // strtok,strcpy
#include <sstream> // stringstream
#include <iomanip>
#include <fnmatch.h> // fnmatch
#include <cctype> // isspace
#include <vector> // vector

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
    // 显示所有的数据
    ALL,
    // 分别从所有字段中找
    GLOBAL,
    // 在 ISBN 字段中查询
    ISBN,
    NAME,
    AUTHOR,
    PUBLISHER,
    CATEGORY
};

/**
 *
 * 分页时每页显示的最多条目
 *
 * 重命名为 per_page，原名 limit_num
 * (修改)
 */
enum per_page {
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
unsigned long long  get_max_book_id();
char * string_to_char(std::string);
book* generate_book_linked_list(unsigned long long &total_item, unsigned long long &total_page, check_condition condition = ALL, unsigned long long page = 1, per_page limit = FIVE, sort_order order = ASC, std::string keyword = "");
void print_book(book *&);
bool book_exists(std::string);
void save_all_books(book *&);
void modify_and_save_book_info(std::string);
template <class T>
void convertFromString(T &value, const std::string &s);
void display_search_book();
int display_search_book_select();
int search_book_work(unsigned long long &total_item, unsigned long long &total_page, check_condition condition = ALL, unsigned long long page = 1, per_page limit = FIVE, sort_order order = ASC, std::string keyword = " ");
void search_book(check_condition condition, bool &switch_search_book_home, bool &search_book);
void delete_book_by_id(unsigned long long book_id);
void exchange_data_for_book(book *&p, book *&q);
short int locale_chinese_string_compare (const std::string& s1, const std::string& s2);
void sort_books(book *&pBookHead, sort_condition sort_by, sort_order order_by);
std::string cstr_to_string(const char* cstr);
unsigned int cstr_to_unsigned_int(const char* cstr);
std::string strip_spaces(std::string& s);
std::string get_one_line(std::ifstream &fin, sort_order order);
std::vector<std::string> split(std::string& input, const std::string& delimiter);
std::string reverse_string(const std::string s);


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
                            case -1: // 返回主页
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

            new_book->isbn = strip_spaces(new_book->isbn);

            if (!new_book->isbn.compare("0")) {
                return 0; // 如果输入 0 ，退出程序
            } else if (!new_book->isbn.compare("-1")) {
                return -1; // 输入1 ，返回管理首页
            }

            //std::cout << book_exists(new_book->isbn) << std::endl;
            //std::exit(0);
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

            new_book->book_name = strip_spaces(new_book->book_name);

            if (!new_book->book_name.compare("")){
                switch_add = true;
            }
        }

        switch_add = true;
        while (switch_add) {
            switch_add = false;

            std::cout << "作者：";
            std::getline(std::cin, new_book->author);

            new_book->author = strip_spaces(new_book->author);

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

            new_book->book_category = strip_spaces(new_book->book_category);

            if (!new_book->book_category.compare(""))
                switch_add = true;

        }

        switch_add = true;
        while (switch_add) {
            switch_add = false;
            std::cout << "出版社：";
            //std::cin >> new_book->book_publisher;
            std::getline(std::cin, new_book->book_publisher);

            new_book->book_publisher = strip_spaces(new_book->book_publisher);
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

            price = strip_spaces(price);

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
            switch_is_to_save = false;
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

// /**
//  *
//  * 组合图书信息
//  * 这里的改变將影响到函数外
//  */
void combine_book_data(book * &pBookHead) {
    
    pBookHead->book_id = get_max_book_id() + 1;
    pBookHead->book_amount++;
    pBookHead->book_current_amount++;
    pBookHead->book_status = 1;
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

void truncate_file(const char* filename) {
    std::ofstream fout(filename, std::ios_base::trunc);
    if (!fout) {
        std::cout << "文件打开失败！" << std::endl;
        std::exit(0);
    }
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
    if (fin.eof()) {
        std::cout << "文件为空，未知错误！" << std::endl;
        std::exit(0);
    }

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

/**
 *
 * 根据传入的参数筛选出符合的条件的条目
 * param total_item     用于计算查询结果总页数
 * param total_page     查询结果的总页数，是一个引用
 * param condition      查询的条件
 * param page           页码
 * param limit          每页最多有几条数据
 * param order          逆序从文件中取数据，还是顺序取数据
 * param keyword        关键词
 * （大修改）
 */
book* generate_book_linked_list(unsigned long long &total_item, unsigned long long &total_page, check_condition condition, unsigned long long page, per_page limit, sort_order order, std::string keyword) {

    book* pHead = nullptr;

    strip_spaces(keyword);

    if (keyword.empty()) {
        if (condition != ALL)
            return  pHead;
    }

    std::ifstream fin;
    fin = get_file_read_handler("books.txt");

    fin.get();
    if (fin.eof()) {
        return pHead;
    }
    fin.seekg(0);
    if (order == DESC) {
        fin = get_file_read_handler("books.txt", 'E');
    }


    unsigned long long items_per_page = limit;
    // 从 0 开始
    unsigned long long this_page_first_item_pos;
    unsigned long long this_page_last_item_pos;
    // 从 0 到 this_page_first_item_pos 需要跳过的计数
    unsigned long long skip_count = 0;
    

    this_page_first_item_pos = (page - 1) * items_per_page;
    this_page_last_item_pos = this_page_first_item_pos + items_per_page - 1;

    // fnmatch() 函数任意位置匹配
    keyword = "*" + keyword + "*";

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

        total_item ++;

        if (skip_count < this_page_first_item_pos) {
            skip_count ++;
            continue;
        }

        // 如果不加上 1，就会出现每页都少 1 项数据
        if (this_page_last_item_pos + 1 == this_page_first_item_pos) {
            continue;
        }
        // 9 8 7 6

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

    total_page = total_item / limit + 1;
    if (total_item % limit == 0)
        total_page --;

    return pHead;
}


// book * generate_book_linked_list() {
//     std::ifstream fin = get_file_read_handler("books.txt");
// 
//     std::string _line;
//     char* delim = string_to_char("|||");
//     char* tmp = nullptr;
//     char* line = nullptr;
//     unsigned short count = 0;
//     book * pHead = nullptr;
//     book * pCurrent = nullptr;
//     book * pLast = nullptr;
//     while (!fin.eof()) {
//         getline(fin, _line);
//         //std::cout << str << endl;
//         line = string_to_char(_line);
//         tmp = std::strtok(line, delim);
//         count = 0;
// 
//         if (tmp != nullptr)
//             pCurrent = new book;
// 
//         while (tmp != nullptr) {
//             //std::cout << tmp << endl;
// 
//             switch (++count) {
//                 case 1:
//                     //cout << "1:" << tmp << endl;
//                     pCurrent->book_id = std::stoull(cstr_to_string(tmp));
//                     // 不知道为何这也行
//                     //pCurrent->book_id = std::stoull(tmp);
//                     break;
//                 case 2:
//                     //cout << "2:" << tmp << endl;
//                     pCurrent->isbn = tmp;
//                     break;
//                 case 3:
//                     //cout << "3:" <<tmp << endl;
//                     pCurrent->book_name = tmp;
//                     break;
//                 case 4:
//                     pCurrent->author = tmp;
//                     break;
//                 case 5:
//                     pCurrent->book_publisher = tmp;
//                     break;
//                 case 6:
//                     pCurrent->book_category = tmp;
//                     break;
//                 case 7:
//                     pCurrent->price = std::atof(tmp);
//                     break;
//                 case 8:
//                     pCurrent->borrow_count = std::stoull(cstr_to_string(tmp));
//                     break;
//                 case 9:
//                     pCurrent->book_amount = cstr_to_unsigned_int(tmp);
//                     break;
//                 case 10:
//                     pCurrent->book_current_amount = cstr_to_unsigned_int(tmp);
//                     break;
//                 case 11:
//                     pCurrent->appointment = std::stoull(cstr_to_string(tmp));
//                     break;
//                 case 12:
//                     //pCurrent->book_status = (bool)tmp;
//                     /*
//                     if (!std::strcmp(tmp, "0"))
//                         pCurrent->book_status = 0;
//                     else
//                         pCurrent->book_status = 1;
//                     */
//                     pCurrent->book_status = cstr_to_unsigned_int(tmp);
// 
//                     break;
//                 case 13:
//                     pCurrent->last_borrow_date = tmp;
//                     break;
//                 case 14:
//                     pCurrent->last_return_date = tmp;
//                     break;
//             }
//             tmp = std::strtok(nullptr, delim);
//         }
// 
// 
//         if (pHead == nullptr) {
//             pHead = pCurrent;
//         } else {
//             pLast->next = pCurrent;
//         }
// 
//         pLast = pCurrent;
// 
//     }
// 
//     /*
//        if (fin.eof()) {
//        cout << "file empty" << endl;
//        }
//        */
// 
//     fin.close();
// 
//     if (pHead != nullptr) {
//         pLast->next = nullptr;
//     }
// 
// 
//     return pHead;
// 
// }

/**
 *
 * 通过 isbn 号查询书籍是否已经存在
 */
bool book_exists(std::string isbn) {

    std::ifstream fin = get_file_read_handler("./books.txt");

    std::string line = get_one_line(fin, ASC);

    std::vector<std::string> vec;
    while (!line.empty()) {
        vec = split(line, "|||");
        if (vec[1] == isbn)
            return true;
        line = get_one_line(fin, ASC);
    }
    return false;

}

/**
 *
 * 修改图书信息
 * 最后保存到文件。
 */
void modify_and_save_book_info(std::string isbn) {
    
    std::ifstream fin;
    std::ofstream fout;
    fin = get_file_read_handler("./books.txt");
    fout = get_file_write_handler("./books.tmp", 'T');
    fout.close();
    fout = get_file_write_handler("./books.tmp", 'A');
    //fout << 111;
    //fout.close();
    //return;

    std::string line = get_one_line(fin, ASC);
    std::vector<std::string> vec;
    while (!line.empty()) {
        vec = split(line, "|||");
        if (vec[1] == isbn) {
            // 7 8 11
            fout << vec[0] << "|||" << vec[1] << "|||" << vec[2] << "|||" << vec[3] << "|||" << vec[4] << "|||" << vec[5] << "|||" << vec[6] << "|||" << 1 + std::stoul(vec[7]) << "|||" << 1 + std::stoul(vec[8]) << "|||" << vec[9] << "|||" << vec[10] << "|||" << "1" << "|||" << vec[12] << "|||" <<  vec[13] << std::endl;
        } else {
            fout << vec[0] << "|||" << vec[1] << "|||" << vec[2] << "|||" << vec[3] << "|||" << vec[4] << "|||" << vec[5] << "|||" << vec[6] << "|||" << vec[7] << "|||" << vec[8] << "|||" << vec[9] << "|||" << vec[10] << "|||" << vec[11] << "|||" << vec[12] << "|||" <<  vec[13] << std::endl;
        }

        line = get_one_line(fin, ASC);
    }

    fout = get_file_write_handler("./books.txt", 'T');
    fout.close();
    fin = get_file_read_handler("./books.tmp");
    fout = get_file_write_handler("./books.txt", 'A');

    line = get_one_line(fin, ASC);
    while (!line.empty()) {
        fout << line << std::endl;
        line = get_one_line(fin, ASC);
    }
    fin.close();
    fout.close();
    

    // 清除文件内容
    fout = get_file_write_handler("./books.tmp", 'T');
    fout.close();

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
int search_book_work(unsigned long long &total_item, unsigned long long &total_page, check_condition condition, unsigned long long page, per_page limit, sort_order order, std::string keyword) {

    strip_spaces(keyword);
    if ((!keyword.compare("")) && (condition != ALL)) {
        // 关键字为空
        std::cout << "关键词为空，请重新输入。" << std::endl;
        return 0;
    }

    book* pBook = generate_book_linked_list(total_item, total_page, condition, page, limit, order, keyword); 

    if (!pBook) {
        std::cout << "非常抱歉！没有找到哦！" << std::endl;
        return 1;
    } else {
        // 找到
        print_book(pBook);
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

    unsigned long long total_item = 0;
    unsigned long long total_page = 0;
    unsigned long long page = 1;
    per_page limit = TWENTY;
    sort_order order = ASC;

    std::string select;

    bool book_operation = true;
    
    switch (search_book_work(total_item, total_page, condition, page, limit, order, keyword)) {
        case 0:
            // 关键词为空
        case 1:
            // 没有找到关键词
            search_book = true;
            break;
        case 2:
            // 找到
            while (book_operation) {
                book_operation = false;
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
                    book_operation = true;
                    std::cout << "成功删除 ID 为 " << i << " 的图书！" << std::endl;
                } else if (!select.compare("-2")) {
                    // 修改操作
                    // 未完成
                }

                //search_book = true;
            }
            break;
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
std::string strip_spaces(std::string& s) {
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

    return s;
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



