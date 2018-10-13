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
 *  * 若在库中找到相同的输入 ISBN 号，提示是否更新相关的信息(自增1)
 *  * 若在库中没有找到相同的 ISBN 号，则直接提示输入其余的图书信息项。
 * 2. 删
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
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <fnmatch.h> // fnmatch

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
    reader * next = NULL;
};

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
    book * next = NULL;
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

void display_header();
int display_home();
int dipslay_admin_home();
int display_logon_reader();
int add_book();
char ensure_book_info(book *);
void save_book(book *, bool);
std::ifstream get_file_read_handler(const char *, const char);
std::ofstream get_file_write_handler(const char *, const char);
void combine_book_data(book * &);
int get_max_book_id(book *);
char * string_to_char(std::string);
book * generate_book_link_table();
void print_book(book *);
bool book_exists(std::string);
void save_all_books(book *&);
void modify_and_save_book_info(std::string);
template <class T>
void convertFromString(T &value, const std::string &s);
int display_search_book();
int search_books(book * pBook, check_condition condition, std::string keyword);

int main() {

    bool switchy_home = true;
    bool search_book = true;
    bool search_book_home = true;
    std::string keyword = "";

    switch (display_home()) {
        case 100:
            // enter management system
            //std::cout << "管理员登录成功" << std::endl;
            while (switchy_home) {
                switchy_home = false;
                switch (dipslay_admin_home()) {
                    case 2:
                        // 增加图书
                        switch (add_book()) {
                            case -1:
                                switchy_home = true;
                                break;
                            case 0:
                                return 0;
                            default:
                                switchy_home = true;
                                break;
                        }
                        break;
                    case 3:
                        // 查询图书
                        while (search_book_home) {
                            search_book_home = false;
                            switch (display_search_book()) {
                                case 200:
                                    // 所有图书
                                    search_books(generate_book_link_table(), ALL, "");
                                    break;
                                case 201:
                                    // 按 ISBN
                                    std::getchar();
                                    search_book = true;
                                    while (search_book) {
                                        search_book = false;
                                        std::cout << "请输入要查询的ISBN(输入-1重新选择):";
                                        //std::cin >> keyword;
                                        std::getline(std::cin, keyword);

                                        if (!keyword.compare("-1")) {
                                            search_book_home = true;
                                            break;;
                                        } 
                                            

                                        switch (search_books(generate_book_link_table(), ISBN, keyword)) {
                                            case 0:
                                                // 关键词为空
                                                search_book = true;
                                                break;
                                            case 1:
                                                // 没有找到关键词
                                                search_book = true;
                                                break;
                                            case 2:
                                                // 找到
                                                break;
                                        }
                                    }
                                    break;
                                case 202:
                                    // 按图书名
                                    break;
                                case 203:
                                    // 按作者
                                    break;
                                case 204:
                                    // 按出版社
                                    break;
                                case 205:
                                    // 按图书种类
                                    break;
                                case 0:
                                    return 0;
                                case 1:
                                    switchy_home = true;
                                    break;
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

}

int dipslay_admin_home() {

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
    std::cout << "+                     |- 修改图书信息  9. 借出图书            |- 删除用户                  +" << std::endl;
    std::cout << "+                     |- 删除图书                             |- 修改用户                  +" << std::endl;
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
}

int add_book() {

    display_header();
    std::cout << "+                                        添加图书                                          +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;

    bool switchy_new = true;
    bool switchy_add = true;
    bool is_break = false;
    while (switchy_new) {

        switchy_new = false;

        book * new_book = new book;

        std::getchar();
        switchy_add = true;
        while (switchy_add) {
            switchy_add = false;

            std::cout << "ISBN(0:退出程序，-1:返回管理首页, Ctrl c 结束程序)：";
            //std::cin >> new_book->isbn;
            std::getline(std::cin, new_book->isbn);

            if (!new_book->isbn.compare("0")) {
                return 0; // 如果输入 0 ，退出程序
            } else if (!new_book->isbn.compare("-1")) {
                return -1; // 输入1 ，返回管理首页
            } 

            if (book_exists(new_book->isbn)) {
                // 存在
                bool switchy_is_update = true;
                while (switchy_is_update) {
                    switchy_is_update = false;
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
                            switchy_is_update = true;
                    }
                    std::cout << "=======================" << std::endl;

                }
                switchy_new = true;
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
                switchy_add = true;
                continue;
            }
        }

        if (is_break) 
            continue;

        switchy_add = true;
        while (switchy_add) {
            switchy_add = false;

            std::cout << "图书名：";
            //std::cin >>  new_book->book_name;
            //std::getline(std::cin, new_book->book_name, '#');
            std::getline(std::cin, new_book->book_name);

            if (!new_book->book_name.compare("")){
                switchy_add = true;
            }
        }

        switchy_add = true;
        while (switchy_add) {
            switchy_add = false;

            std::cout << "作者：";
            std::getline(std::cin, new_book->author);

            if (!new_book->author.compare("")) {
                switchy_add = true;
            }
        }

        switchy_add = true;
        while (switchy_add) {
            switchy_add = false;
            std::cout << "图书类别：";
            //std::cin >> new_book->book_category;
            std::getline(std::cin, new_book->book_category);

            if (!new_book->book_category.compare(""))
                switchy_add = true;

        }

        switchy_add = true;
        while (switchy_add) {
            switchy_add = false;
            std::cout << "出版社：";
            //std::cin >> new_book->book_publisher;
            std::getline(std::cin, new_book->book_publisher);
            
            if (!new_book->book_publisher.compare("")) 
                switchy_add = true;
        }

        switchy_add = true;
        std::string price;
        while (switchy_add) {
            switchy_add = false;
            //std::cout << "图书价格请务必输入数字，否则将可能出现无法意料的故障！" << std::endl;
            std::cout << "图书价格：";
            std::getline(std::cin, price);

            if (!price.compare("")) 
                switchy_add = true;
        }

        convertFromString(new_book->price, price);
        

        std::cout << "=======================" << std::endl;

        bool switchy_is_to_save = true;
        
        while (switchy_is_to_save) {
            switchy_is_to_save = false;
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
                    switchy_is_to_save = true;
                    std::cout << "您在说什么？我听不明白！" << std::endl;
                    std::cout << "请重新输入！" << std::endl;
                    break;
            }

            std::cout << "==============================" << std::endl;
        }
        switchy_new = true;
    }
}

char ensure_book_info(book * pBook) {

    std::cout << "请确定录入以下的信息：" << std::endl;
    std::cout << "ISBN：" << pBook->isbn << std::endl;
    std::cout << "图书名：" << pBook->book_name << std::endl;
    std::cout << "作者：" << pBook->author << std::endl;
    std::cout << "图书类别：" << pBook->book_category << std::endl;
    std::cout << "出版社：" << pBook->book_publisher << std::endl;
    std::cout << "价格：" << pBook->price << std::endl;

    std::cout << "确定保存(Y/N)：";
    char ensure = 'N';
    std::cin >> ensure;

    return ensure;
} 

/**
 * 
 * 保存图书信息
 */

void save_book(book * pBook, bool single_book = true) {

    std::ofstream fout = get_file_write_handler("books.txt", 'A');

    fout << pBook->book_id << "|||" << pBook->isbn << "|||" << pBook->book_name  << "|||" << pBook->author << "|||" << pBook->book_publisher << "|||" <<  pBook->book_category << "|||" << pBook->price << "|||" << pBook->borrow_count << "|||" << pBook->book_amount << "|||" << pBook->book_current_amount << "|||" << pBook->appointment << "|||" << pBook->book_status << "|||" << pBook->last_borrow_date << "|||" << pBook->last_return_date << std::endl;

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
void combine_book_data(book * &pBook) {
    book * pHead = generate_book_link_table();
    pBook->book_id = get_max_book_id(pHead) + 1;
    pBook->book_amount++;
    pBook->book_current_amount++;
    pBook->book_status = true;
}

/**
 *
 * 获取最大图书 id，
 * 从而得到分配给新增图书 id。
 */
int get_max_book_id(book * pHead) {

    if (pHead == NULL) {
        return 0;
    }

    book * pTmp = NULL;
    while (pHead != NULL) {

        pTmp = pHead;
        pHead = pHead->next;

        if (pHead == NULL) {
            //cout << "Last 1:" << pTmp->book_id << endl;
            return pTmp->book_id;
        }
    }
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

book * generate_book_link_table() {
    std::ifstream fin = get_file_read_handler("books.txt");

    std::string _line;
    char* delim = string_to_char("|||");
    char* tmp;
    char* line;
    unsigned short count = 0;
    book * pHead = NULL;
    book * pCurrent = NULL;
    book * pLast = NULL;
    while (!fin.eof()) {
        getline(fin, _line);
        //std::cout << str << endl;
        line = string_to_char(_line);
        tmp = std::strtok(line, delim);
        count = 0;
        
        if (tmp != NULL) 
            pCurrent = new book;

        while (tmp != NULL) {
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
            tmp = std::strtok(NULL, delim);
        }


        if (pHead == NULL) {
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

    if (pHead != NULL) {
        pLast->next = NULL;
    }


    return pHead;

}

bool book_exists(std::string isbn) {

    book * pBook = generate_book_link_table();
    while (pBook != NULL) {
        // 相等为 0
        if (!pBook->isbn.compare(isbn)) {
            return true;
        }
        pBook = pBook->next;
    }

    return false;

}

/**
 *
 * 修改图书信息
 * 最后保存到文件。
 */
void modify_and_save_book_info(std::string isbn) {
    book * pBook = generate_book_link_table();

    book * pCurr = pBook;

    while (pCurr != NULL) {

        if (!pCurr->isbn.compare(isbn)) {
            // 找到
            // 更新一些信息
            // 然后把新的信息放回到原链表中
            
            pCurr->book_amount++;
            pCurr->book_current_amount++;
            pCurr->book_status = true;
            save_all_books(pBook);
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
    get_file_write_handler("books.txt", 'T').close();

    while (pHead != NULL) {

        save_book(pHead, false);

        pHead = pHead->next;
    }
}

 
void print_book(book * pBook) {
    std::cout << pBook->book_id << std::endl;
}

int display_search_book() {

    display_header();
    std::cout << "+                                        查询图书                                          +" << std::endl;
    std::cout << "+                                    +++++++++++++++                                       +" << std::endl;
    std::cout << "+                                                                                          +" << std::endl;

    std::cout << "+ 可选择以下的查询条件进行查询：200:所有图书 201:ISBN 202:图书名" << std:: endl;
    std::cout << "+                               203:作者 204:出版社 205:图书种类" << std:: endl;
    std::cout << "+ 请选择查询条件 ID（输入 0 退出系统，输入 1 返回管理首页）：";
    std::cin >> select_num;

    return select_num;

}

/**
 * 显示所有图书
 * \param pBook 图书结构的指针
 * \param condition 查询条件，有 ISBN、图书名、作者、出版社、图书种类 几种条件，支持模糊搜索
 * \param keyword 查询关键词
 * \return 0 重新输入关键字 1 没有找到关键词
 */
int search_books(book * pBook, check_condition condition = ALL, std::string keyword = "") {

    if ((!keyword.compare("")) && (condition != ALL)) {
        // 关键字为空
        std::cout << "关键词为空，请重新输入。" << std::endl;
        return 0;
    }

    bool found = false;
    while (pBook != NULL) {
        std::stringstream ss_keyword("");

        switch (condition) {
            case ALL:
                // 所有图书列表
                break;
            case ISBN:
                // 按 ISBN 查询
                int ret;
                ss_keyword << '*' << keyword << "*";
                //std::cout << ss_keyword.str() << std::endl;
                //std::cout << pBook->isbn << std::endl;
                if (!fnmatch(ss_keyword.str().c_str(), pBook->isbn.c_str(), FNM_NOESCAPE | FNM_CASEFOLD)) {
                    // fnmatch 返回值为 0 时表示匹配到
                    std::cout << "找到" << pBook->isbn << std::endl;
                    found = true;       
                }
                break;
            case NAME:
                // 按图书名查询
                break;
            case AUTHOR:
                // 按作者查询
                break;
            case PUBLISHER:
                // 按出版社查询
                break;
            case CATEGORY:
                // 按图书种类查询
                break;
            default:
                // ALL，显示所有图书
                break;
                
        }
        pBook = pBook->next;

    }

    if (!found) {
        std::cout << "非常抱歉！没有找到哦！" << std::endl;
        return 1;
    }



    //std::cout << "Id" << "     " << "ISBN" << "     " << "图书名" << "     " << "作者" << "     " << "出版社" << "     " << "图书种类" << "     " << "图书价格" << "     " << "借阅量" << "     " << "总库存量" << "     " << "现库存量" << "     " << "预约量" << "     " << "是否可借" << "     " << "最近一次借出时间" << "     " << "最近一次归还时间" << std::endl; 

    //unsigned short row, column;
    //row = 0;
    //column = 0;
    //
    
    //TextTable t( '-', '|', '+' );

    /*
    while (pBook != NULL) {
        std::cout << std::setw(10) <<  std::setiosflags(std::ios::right)<< pBook->book_id;
        std::cout << std::setw(20) <<  std::setiosflags(std::ios::right)<< pBook->isbn;
        std::cout <<std::setw(90) << std::setiosflags(std::ios::left)<< pBook->book_name;
        std::cout << std::setw(20) <<  std::setiosflags(std::ios::right)<<pBook->author;
        std::cout << std::setw(50) <<  std::setiosflags(std::ios::right)<<pBook->book_publisher;
        std::cout << std::setw(20) <<  std::setiosflags(std::ios::right)<< pBook->book_category;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->price;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->borrow_count;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->book_amount;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->book_current_amount;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->appointment;
        std::cout << std::setw(5) <<  std::setiosflags(std::ios::right)<<pBook->book_status;
        std::cout << std::setw(20) <<  std::setiosflags(std::ios::right)<<pBook->last_borrow_date;
        std::cout << std::setw(20) <<  std::setiosflags(std::ios::right)<<pBook->last_return_date;
        std::cout << std::endl;
        std::cout << "        --------------------------------------        " << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        
        //for (int column = 0; column < 15; column ++) {
            //std::cout << "Id" << "     " << "ISBN" << "     " << "图书名" << "     " << "作者" << "     " << "出版社" << "     " << "图书种类" << "     " << "图书价格" << "     " << "借阅量" << "     " << "总库存量" << "     " << "现库存量" << "     " << "预约量" << "     " << "是否可借" << "     " << "最近一次借出时间" << "     " << "最近一次归还时间" << std::endl; 
            //if (row == 0) {
            //    switch (column) {
            //        case 0:
            //            table[row][column] = "Id";
            //            break;
            //        case 1:
            //            table[row][column] = "ISBN";
            //            break;
            //        case 2:
            //            table[row][column] = "图书名";
            //            break;
            //        case 3:
            //            table[row][column] = "作者";
            //            break;
            //        case 4:
            //            table[row][column] = "出版社";
            //            break;
            //        case 5:
            //            table[row][column] = "图书种类";
            //            break;
            //        case 6:
            //            table[row][column] = "图书价格";
            //            break;
            //        case 7:
            //            table[row][column] = "借阅量";
            //            break;
            //        case 8:
            //            table[row][column] = "总库存量";
            //            break;
            //        case 9:
            //            table[row][column] = "现库存量";
            //            break;
            //        case 10:
            //            table[row][column] = "预约量";
            //            break;
            //        case 11:
            //            table[row][column] = "是否可借";
            //            break;
            //        case 12:
            //            table[row][column] = "最近一次借出时间";
            //            break;
            //        case 13:
            //            table[row][column] = "最近一次归还时间";
            //            break;
            //    }
            //} 
        //}

    


        //std::cout << "Id:" << pBook->book_id << std::endl;
        //std::cout << "ISBN:" << pBook->isbn << std::endl; 
        //std::cout << "图书名:" << pBook->book_name << std::endl;
        //std::cout << "作者:" << pBook->author << std::endl;
        //std::cout << "出版社:" << pBook->book_publisher << std::endl;
        //std::cout << "图书种类:" <<  pBook->book_category << std::endl;
        //std::cout << "图书价格:" << pBook->price << std::endl;
        //std::cout << "借阅量:" << pBook->borrow_count << std:: endl;
        //std::cout << "总库存量:" << pBook->book_amount << std::endl;
        //std::cout << "现库存量:" << pBook->book_current_amount << std::endl;
        //std::cout << "预约量:" << pBook->appointment << std::endl;
        //std::cout << "是不可借:" << pBook->book_status << std::endl;
        //std::cout << "最近一次借出时间:" << pBook->last_borrow_date << std::endl;
        //std::cout << "最近一次归还时间:" << pBook->last_return_date << std::endl;
        //std::cout << std::endl;
        //std::cout << "        --------------------------------------        " << std::endl;
        //std::cout << std::endl;

        pBook = pBook->next;
    }    
    */

    //std::cout << t;
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



