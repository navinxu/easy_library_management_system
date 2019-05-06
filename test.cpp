/*================================================================
*   Copyright (C) 2018 Navin Xu. All rights reserved.
*   
*   Filename    ：test.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2018年10月10日
*   Description ：
================================================================*/
#include <iostream>
#include <string>
#include <locale> // setlocale, locale
#include <algorithm> // sort
#include <cstring> // strcoll
#include <vector> // vector

using  namespace std;

bool localeLessThan (const string& s1, const string& s2) {

   const collate<char>& col = 
       use_facet<collate<char> >(locale()); // Use the global locale

   const char* pb1 = s1.data();
   const char* pb2 = s2.data();

   return (col.compare(pb1, pb1 + s1.size(),
                       pb2, pb2 + s2.size()) < 0);
   //return col.compare(pb1, pb1 + s1.size(),
   //                    pb2, pb2 + s2.size());
}

/**
 *
 * s1 > s2 ret 1
 * s1 < s2 ret -1
 * s1 = s2 ret 0
 * 小写字母 < 大写字母
 * 数字：55 < 6
 */
short int localeLessThan2 (const string& s1, const string& s2) {

   const collate<char>& col = 
       use_facet<collate<char> >(locale()); // Use the global locale

   const char* pb1 = s1.data();
   const char* pb2 = s2.data();

   //return (col.compare(pb1, pb1 + s1.size(),
   //                    pb2, pb2 + s2.size()) < 0);
   return col.compare(pb1, pb1 + s1.size(),
                       pb2, pb2 + s2.size());
}

// 需包含locale、string头文件、使用setlocale函数。
std::wstring StringToWstring(const std::string str)
{// string转wstring
    unsigned len = str.size() * 2;// 预留字节数
    setlocale(LC_CTYPE, "");     //必须调用此函数
    wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
    mbstowcs(p,str.c_str(),len);// 转换
    std::wstring str1(p);
    delete[] p;// 释放申请的内存
    return str1;
}


int main() {

    std::wstring chinese = StringToWstring("我是中国人。");
    std::wstring chinese2 = StringToWstring("一你是中国人。");
    //std::wcout << (chinese.compare(chinese2)) << std::endl;
    std::wcout << chinese << std::endl;

    std::string str1 = "中国人民解放一";
    std::string str2 = "中国人民解放二";

    locale::global(locale("zh_CN.UTF-8"));

    std::cout << "***************" << std::endl;
    std::cout << str1 << std::endl;
    std::cout << str2 << std::endl;
    std::cout << localeLessThan2(str1, str2) << std::endl;
    std::cout << "===============" << std::endl;
    //return 0;
    std::vector<std::string> vstr;
    vstr.push_back(str1);
    vstr.push_back(str2);

    //std::setlocale(LC_ALL, "");
    //std::sort(vstr.begin(), vstr.end());

    std::cout << vstr[0] << std::endl;
    std::cout << vstr[1] << std::endl;

    std::cout << "===============" << std::endl;

    sort(vstr.begin(), vstr.end(), localeLessThan);


    std::cout << vstr[0] << std::endl;
    std::cout << vstr[1] << std::endl;

    std::cout << "===============" << std::endl;

    std::cout << std::locale("zh_CN.UTF-8").name() << std::endl;



    std::wstring wstr123 = StringToWstring("中国人");

    std::wcout << wstr123 << std::endl;
    std::wcout << "+++++++++++++++++++++++" << std::endl;

    

}

