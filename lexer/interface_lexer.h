#pragma once

#include<string>
#include<stdint.h>
#include<type_traits>
#include<iterator>
#include<tuple>
#include<utility>
#include<string_view>
#include "../thirdparty/ascii.h"

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

template <size_t kCount>
struct template_str_buffer
{
    using char_type = char;

    consteval template_str_buffer(const char_type(&str)[kCount]) noexcept;

    char_type data[kCount];
    constexpr static size_t count = kCount - sizeof(char_type);
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
    constexpr LexerIterator(cestr str);
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

template <template_str_buffer kStrBuf>
consteval auto operator""_lex();
