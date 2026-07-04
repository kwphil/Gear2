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
/// @date June 30, 2026 | July 3rd, 2026
/// @file implementation for include/gearlang/lex.hpp

#include <algorithm>
#include <cctype>
#include <cstring>
#include <gearlang/lex.hpp>
#include <string>
#include <vector>
using namespace gearlang::lex;

/// @brief The classification of each char (handy for classifying tokens)
enum class CharType {
        Alpha,
        Num,
        Sym,
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

TokenList Lexer::tokenize() {
        size_t index = 0;
        std::string curr_content;
        std::vector<Token> tokens;

        bool is_string = false;

        CharType prev_char = CharType::Invalid;
        TokenType classify = TokenType::INVALID;

        Span span = {0, 0, file_name};

        while (contents[index] != '\0') {
                char c = contents[index++];
                CharType curr_char = CharType::Invalid;

                auto next_char = [&]() {
                        curr_content.push_back(c);
                        span.end++;
                        prev_char = curr_char;
                };

                if(is_string && c == '"') {
                        next_char();
                        tokens.push_back(flush(curr_content, span, classify));
                        is_string = false;
                        continue;
                }
         
                if (std::isalpha(c))
                        curr_char = CharType::Alpha;
                else if (std::isdigit(c))
                        curr_char = CharType::Num;
                else if (c == '"')
                        is_string = true;

                // if there's a whitespace, then flush
                if (whitespace.contains(c) && !is_string) {
                        if (!curr_content.empty())
                                tokens.push_back(flush(curr_content, span, classify));

                        prev_char = CharType::Invalid;
                        span.start++;
                        span.end++;
                        continue;
                }

                // boundaries
                if (!curr_content.empty() && curr_char != prev_char && !is_string) {
                        tokens.push_back(flush(curr_content, span, classify));
                }

                if (curr_content.empty())
                        span.start = span.end;
                
                classify = classification(curr_content);

                next_char();
        }

        // make sure last token gets flushed
        if (!curr_content.empty())
                tokens.push_back(flush(curr_content, span, classify));

        return TokenList(tokens, tokens.size() - 1);
}
