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
/// @file Tokenizer interface

#pragma once

#include <cstddef>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>
#include "files.hpp"

namespace gearlang::lex {
        /// @brief Denotes the location of the token
        struct Span {
                size_t start; /// @brief Zero-based index of where the token begins
                size_t end; /// @brief Zero-based index of where the token ends 
                const char* file_name; /// @brief the *ABSOLUTE* file path of where the token is
        };

        /// @brief Long laundry list of the different types of tokens
        enum class TokenType {
                IDENTIFIER,
                NUMBER,
                INVALID
        };

        /// @brief I sure hope you know what a token is
        struct Token {
                Span span; /// @brief location of the token
                std::string content; /// @brief What is contained in this token
                TokenType type; /// @brief What the lexer thinks this is
        };

        /// @brief List of tokens
        class TokenList {
        private:
                /// @brief The current array index of where the token is
                size_t index;
                /// @brief The number of tokens
                size_t count;
                /// @brief The token list
                std::vector<Token> tokens;

        public:
                TokenList(std::vector<Token> tokens, size_t count) 
                : index(0), count(count), tokens(tokens) { }

                /// @brief Returns the current token
                /// @note Returns a nullptr when there are no more tokens
                /// @returns The current token
                inline const Token* peek() { 
                        if(index > count) return nullptr;
                        return &tokens[index];
                }
                
                /// @brief Returns the current token and bumps the index
                /// @note Returns a nullptr when there are no more tokens
                /// @returns The current token
                inline const Token* pop() {
                        const Token* tok = peek();
                        if(tok) index++;
                        return tok;
                }
        };

        /// @brief file->tokens
        /// @note Create a new Lexer object for each file opened
        class Lexer {
        private:
                FILE_STREAM contents;
                const char* file_name;

        public:
                Lexer(FILE_STREAM contents, const char* file_name)
                : contents(contents), file_name(file_name) { }

                /// @brief Converts the current stream of chars into a list of tokens 
                TokenList tokenize();
        };
}
