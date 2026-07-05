/*
   _____                 _                       
  / ____|               | |                      
 | |  __  ___  __ _ _ __| |     __ _ _ __   __ _ 
 | | |_ |/ _ \/ _` | '__| |    / _` | '_ \ / _` | Clean, Clear and Fast Code
 | |__| |  __/ (_| | |  | |___| (_| | | | | (_| | https://github.com/kwphil/gearlang
  \_____|\___|\__,_|_|  |______\__,_|_| |_|\__, |
                                            __/ |
                                           |___/ 

Licensed under the MIT License <https://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/// @author Kora - em: phillipsw1980@gmail.com, gh: kwphil
/// @date June 30, 2026 | July 4th, 2026
/// @file implementation for include/gearlang/lex.hpp

#include <algorithm>
#include <cctype>
#include <cstring>
#include <gearlang/lex.hpp>
#include <string>
#include <vector>
#include <functional>
using namespace gearlang::lex;

/// @brief The classification of each char (handy for classifying tokens)
enum class CharType {
        Alpha,
        Num,
        Sym,
        DQuot,
        Invalid,
};

/// @brief Just a list of common whitespace chars
std::string whitespace = " \r\n\t";

/// @brief Function that creates a new token and resets for the next token
Token flush(std::string& content, Span& span, TokenType classify) {
        Token tok;
        tok.content = content;
        tok.span = span;
        tok.type = classify;

        span.end++;
        span.start = span.end;
        content.clear();

        return tok;
}

TokenType classification(std::string& content) {
        if(content[0] == '"') return TokenType::STRING;
        if(std::ranges::all_of(content, [](unsigned char c){
                return isalpha(c) || c == '_';
        })) return TokenType::IDENTIFIER;
        if(std::ranges::all_of(content, [](unsigned char c){
                return isdigit(c) || c == '.';
        })) return TokenType::NUMBER;
        return TokenType::INVALID;
}

std::string read_while(std::ifstream& stream, std::function<bool(char)> check) {
        std::string buf = "";

        char ch = stream.get();

        do {
                buf.push_back(ch);
        } while(check(ch));

        return buf;
}

Token Lexer::request_next() {
        char c = contents->get();

        std::string str;
        str = c;
        Span span = { index, index+1, file_name };
        TokenType classify = TokenType::INVALID;

        CharType cty = CharType::Invalid;

        // classify
        if(isalpha(c) || c == '_') { cty = CharType::Alpha; }
        if(isdigit(c)) { cty = CharType::Num; }
        if(c == '"') { cty = CharType::DQuot; }

        switch(cty) {
                case CharType::Alpha:
                        classify = TokenType::IDENTIFIER;
                        str = read_while(*contents, [](char ch){ return ch == '_' || isalpha(ch); });
                        break;
                case CharType::Num:
                        classify = TokenType::NUMBER;
                        str = read_while(*contents, [](char ch){ return ch == '.' || isdigit(ch); });
                        break;
                case CharType::DQuot:
                        classify = TokenType::STRING;
                        str = read_while(*contents, [](char ch){ return ch != '"'; });
                        break;
                default:
                        break;
        };

        return Token{ span, str, classify };
}
