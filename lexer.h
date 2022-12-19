#pragma once

#include<string>
#include<stdint.h>
#include<type_traits>
#include<iterator>
#include<tuple>
#include<utility>
#include<string_view>
#include "thirdparty/ascii.h"

// Лексический анализатор возвращает токены [0-255], если это неизвестны, 
// иначе одну из известных единиц кода
enum class Token : int32_t
{
  eof = -1,

  // команды (ключевые слова)
  _begin = -2, _end = -3,
  _procedure = -4, _function = -5,
  _while = -6, _do = -7, _for = -8,
  _var = -9, _to = -10, _if = -11,
  _then = -12, _else = -13,


  // операнды (идентификаторы, числа)
  identifier = -14, rnumber = -15,
  inumber = -16
};

using cestr = std::string_view;

class LexerIterator
{
    cestr::iterator it;

    cestr IdentifierStr;
    double RealVal;
    int IntVal;
    int LastChar;
    Token tok;

    constexpr std::tuple<cestr, double, int, int, Token> getLexem();
public:
    constexpr LexerIterator(cestr str)
    {
        it = std::begin(str);
    }
    constexpr LexerIterator& operator++();
    constexpr LexerIterator operator++(int);
    constexpr bool operator==(LexerIterator other) const; 
    constexpr bool operator!=(LexerIterator other) const;
    constexpr const std::tuple<cestr, double, int, int, Token> operator*();
    // iterator traits
    using difference_type = long;
    using value_type = std::tuple<cestr, double, int, int, Token>;
    using pointer = const std::tuple<cestr, double, int, int, Token>*;
    using reference = const std::tuple<cestr, double, int, int, Token>&;
    using iterator_category = std::input_iterator_tag;

};



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

