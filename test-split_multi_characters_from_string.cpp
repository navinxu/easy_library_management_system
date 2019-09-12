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
 *
 * 分割由多个字符相隔的字符串
vector<string> split(string& input, const string& delimiter) {
    vector<string> result;

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

int main() {

    /*
    string s = "Hello";
    cout << s.find("ll") << endl;
    cout << s.find(",") <<endl;
    cout << s.npos <<endl;
    cout << string::npos <<endl;
    */
    string s("Hello, World");
    vector<string> vecs = split(s, ", ");

    for (auto s : vecs)
        cout << s << endl;


    return 0;
}
