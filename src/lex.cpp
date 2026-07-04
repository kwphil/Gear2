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
/// @date June 30, 2026
/// @file implementation for include/gearlang/lex.hpp

#include <cctype>
#include <cstring>
#include <gearlang/lex.hpp>
#include <string>
#include <vector>
using namespace gearlang::lex;

enum class CharType {
        Alpha,
        Num,
        Invalid,
};

std::string whitespace = " \0\r\n\t";

Token flush(std::string& content, Span& span, TokenType classify) {
        Token tok;
        tok.content = content;
        tok.span = span;
        tok.type = classify;

        span.start = (span.end += 1);
        content.clear();

        return tok;
}

TokenList Lexer::tokenize() {
    size_t index = 0;
    std::string curr_content;
    std::vector<Token> tokens;

    CharType prev_char = CharType::Invalid;
    TokenType classify = TokenType::INVALID;

    Span span = {0, 0, file_name};

    while (contents[index] != '\0') {
        char c = contents[index++];
        CharType curr_char = CharType::Invalid;

        if (std::isalpha(static_cast<unsigned char>(c)))
            curr_char = CharType::Alpha;
        else if (std::isdigit(static_cast<unsigned char>(c)))
            curr_char = CharType::Num;

        // if there's a whitespace, then flush
        if (whitespace.contains(c)) {
            if (!curr_content.empty())
                tokens.push_back(flush(curr_content, span, classify));

            prev_char = CharType::Invalid;
            span.start++;
            span.end++;
            continue;
        }

        // boundaries
        if (!curr_content.empty() && curr_char != prev_char) {
            tokens.push_back(flush(curr_content, span, classify));
        }

        if (curr_content.empty())
            span.start = span.end;

        curr_content.push_back(c);
        span.end++;

        switch (curr_char) {
            case CharType::Alpha:
                classify = TokenType::IDENTIFIER;
                break;

            case CharType::Num:
                classify = TokenType::NUMBER;
                break;

            default:
                classify = TokenType::INVALID;
                break;
        }

        prev_char = curr_char;
    }

    // make sure last token gets flushed
    if (!curr_content.empty())
        tokens.push_back(flush(curr_content, span, classify));

    return TokenList(tokens, tokens.size() - 1);
}
