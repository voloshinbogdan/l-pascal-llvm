#pragma once

#include"interface_lexer.h"

template<size_t kCount>
consteval template_str_buffer<kCount>::template_str_buffer(const char_type(&str)[kCount]) noexcept
{
    for (size_t i = 0; i < kCount; ++i) {
        data[i] = str[i];
    }
}

constexpr LexerIterator::LexerIterator(cestr str)
{
    it = std::begin(str);
}

constexpr LexerIterator& LexerIterator::operator++()
{
    std::tie(IdentifierStr, RealVal, IntVal, LastChar, tok) = getLexem();
    return *this;
}

constexpr LexerIterator LexerIterator::operator++(int)
{
    auto ret = *this;
    std::tie(IdentifierStr, RealVal, IntVal, LastChar, tok) = getLexem();
    return ret;
}

constexpr bool LexerIterator::operator==(LexerIterator other) const
{
    return it == other.it;
}

constexpr bool LexerIterator::operator!=(LexerIterator other) const
{
    return ! (*this==other);
}

constexpr const std::tuple<cestr, double, int, int, Token> LexerIterator::operator*()
{
    return { std::forward<decltype(IdentifierStr)>(IdentifierStr), std::forward<decltype(RealVal)>(RealVal), 
        std::forward<decltype(IntVal)>(IntVal), std::forward<decltype(LastChar)>(LastChar), std::forward<decltype(tok)>(tok) };
}

constexpr std::tuple<cestr, double, int, int, Token> LexerIterator::getLexem()
{
    namespace sa = std::ascii;
    while(sa::isspace(*it))
        it++;

    if(sa::isalpha(*it))
    {
        int i = 1;
        auto be_it = it;
        while (sa::isalnum(*it))
        {
            i++;
            it++;
        }
        auto ident = std::string_view(be_it, i);
        Token tok;
        if (IdentifierStr == "begin") tok = Token::_begin;
        else if (IdentifierStr == "end") tok = Token::_end;
        else if (IdentifierStr == "procedure") tok = Token::_procedure;
        else if (IdentifierStr == "function") tok = Token::_function;
        else if (IdentifierStr == "if") tok = Token::_if;
        else if (IdentifierStr == "then") tok = Token::_then;
        else if (IdentifierStr == "else") tok = Token::_else;
        else if (IdentifierStr == "while") tok = Token::_while;
        else if (IdentifierStr == "do") tok = Token::_do;
        else if (IdentifierStr == "for") tok = Token::_for;
        else if (IdentifierStr == "to") tok = Token::_to;
        else if (IdentifierStr == "var") tok = Token::_var;
        else tok = Token::identifier;
        return { ident, 0.0, 0, *it, tok };
    }
    if (sa::isdigit(*it))
    {
        int d = 0;
        do {
            d = d*10 + sa::todigit(*it);
            it++;
        } while (sa::isdigit(*it));

        return { "", 0.0, d, *it, Token::inumber };
    }
    if (*it == EOF || *it == '\0')
    {
        return { "", 0.0, 0, *it, Token::eof };

    }
    Token tok = (Token)*it;
    return { "", 0.0, 0, *it, tok };
}


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
