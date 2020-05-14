/*================================================================
*   Copyright (C) 2019 Navin Xu. All rights reserved.
*   
*   Filename    ：test-convert_string_obj_to_unsigned_long_long_type.cpp
*   Author      ：Navin Xu
*   E-Mail      ：admin@navinxu.com
*   Create Date ：2019年08月07日
*   Description ：
================================================================*/

// stoull example
#include <iostream>   // std::cout
#include <string>     // std::string, std::stoull

int main ()
{
  std::string str = "8246821 0xffff 020 -1";

  std::string::size_type sz = 0;   // alias of size_t

  while (!str.empty()) {
    unsigned long long ull = std::stoull (str,&sz,0);
    std::cout << str.substr(0,sz) << " interpreted as " << ull << '\n';
    str = str.substr(sz);
  }

  return 0;
}
