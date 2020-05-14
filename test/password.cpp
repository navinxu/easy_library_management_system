/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：password.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月09日
*   Description ：
================================================================*/
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main (void)
{
    char *ch;//返回值应为字符串首地址
    ch = getpass("Please input the password:\n");
    //printf("password: %s",ch);
    cout << ch << endl;
    

    return 0;
}
