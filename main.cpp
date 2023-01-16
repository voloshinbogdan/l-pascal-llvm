#include<iostream>
#include<string>
#include<string_view>
#include<utility>
#include<algorithm>
#include<array>
#include<functional>
#include<utility>
#include"lexer/lexer.h"

int main(int argc[[maybe_unused]], char** argv[[maybe_unused]]){
    constexpr auto v = "abc 34"_lex;
    for (auto &i: v)
    {
        auto [id, rn, in, lc, t] = i;
        std::cout << id << " " << in << " " << (int)t << std::endl;
    }
}
