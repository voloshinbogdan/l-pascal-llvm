#include<iostream>
#include <string>
#include<string_view>
#include<utility>
#include<algorithm>
#include<array>
#include<functional>
#include<utility>
#include"lexer.h"

template <size_t kCount>
struct template_str_buffer
{
    using char_type = char;

    consteval template_str_buffer(const char_type(&str)[kCount]) noexcept
    {
        for (size_t i = 0; i < kCount; ++i) {
            data[i] = str[i];
        }
    }

    char_type data[kCount];
    constexpr static size_t count = kCount - sizeof(char_type);
};


constexpr auto _CountLexems(std::string_view s)
{
    LexerIterator li(s);
    int i = 0;
    Token tok;
    do {
        ++li;
        auto v = *li;
        tok = std::get<Token>(v);
        i++;
    } while(tok != Token::eof);

    return i;
}
template<int N>
constexpr auto _RunLexer(std::string_view s)
{
    LexerIterator li(s);
    ++li;
    std::array<LexerIterator::value_type, N> res;

    res[0] = *li;
    for (size_t i = 1; i < N; ++i)
    {
        li++;
        res[i] = *li;
    }

    return res;
}

using namespace std::literals;

template <template_str_buffer kStrBuf>
consteval auto operator""_lex()
{
    constexpr auto vc = _CountLexems(kStrBuf.data);
    return _RunLexer<vc>(kStrBuf.data);
}



int main(int argc[[maybe_unused]], char** argv[[maybe_unused]]){
    constexpr auto v = "abc 34"_lex;
    for (auto &i: v)
    {
        auto [id, rn, in, lc, t] = i;
        std::cout << id << " " << in << " " << (int)t << std::endl;
    }
}
