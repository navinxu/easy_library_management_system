/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-split_multi_characters_from_string.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月21日
*   Description ： 分割由多个字符为分隔符组成的 string 字符串
================================================================*/
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * 这里有个要注意的地方是，如果想得到分隔符后面的最后一个字符串，
 * 那么就必须在整个字符串的末尾加上同样的分隔符
 */
vector<string> split(const string& input, const string& delimiter) {
    vector<string> result;
    std::string::size_type pos = 0, delim_pos = 0;

    // string.find 在找不到时返回 string::npos 值
    while (delim_pos != input.npos) {
        // 找到的分隔符的第一个字符位置
        delim_pos = input.find(delimiter, pos);
        if (delim_pos != string::npos) {
            // 此时游标的指向位置
            result.push_back(input.substr(pos,delim_pos - pos));
        } else
            break;
        pos = delim_pos + delimiter.length();

    }


    return result;
}

int main() {

    /*
    string s = "Hello";
    cout << s.find("ll") << endl;
    cout << s.find(",") <<endl;
    cout << s.npos <<endl;
    cout << string::npos <<endl;
    */
    string s("Hello, World, ");
    vector<string> vecs = split(s, ", ");

    for (auto s : vecs)
        cout << s << endl;


    return 0;
}
