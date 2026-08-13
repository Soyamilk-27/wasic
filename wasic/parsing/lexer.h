#pragma once
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

enum class TokenKind {
    Identifier,
    Number,

    Fn,
    End,

    LParen,
    RParen,
    LBracket,
    RBracket,

    Equal,
    Plus,
    Minus,
    Star,
    Slash,

    Newline,
    Eof
};

struct Token {
    TokenKind kind;
    std::string text;
};

static const char* tokenName(TokenKind kind) {
    switch (kind) {
        case TokenKind::Identifier: return "Identifier";
        case TokenKind::Number:     return "Number";
        case TokenKind::Fn:         return "Fn";
        case TokenKind::End:        return "End";
        case TokenKind::LParen:     return "LParen";
        case TokenKind::RParen:     return "RParen";
        case TokenKind::LBracket:   return "LBracket";
        case TokenKind::RBracket:   return "RBracket";
        case TokenKind::Equal:      return "Equal";
        case TokenKind::Plus:       return "Plus";
        case TokenKind::Minus:      return "Minus";
        case TokenKind::Star:       return "Star";
        case TokenKind::Slash:      return "Slash";
        case TokenKind::Newline:    return "Newline";
        case TokenKind::Eof:        return "Eof";
    }

    return "Unknown";
}

class Lexer {
public:
    explicit Lexer(const std::string& source)
        : source(std::move(source)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (!atEnd()) {
            char c = peek();

            if (c == ' ' || c == '\t' || c == '\r') {
                advance();
                continue;
            }

            if (c == '\n') {
                tokens.push_back({TokenKind::Newline, "\\n"});
                advance();
                continue;
            }

            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                tokens.push_back(identifier());
                continue;
            }

            if (std::isdigit(static_cast<unsigned char>(c))) {
                tokens.push_back(number());
                continue;
            }

            switch (c) {
                case '(':
                    tokens.push_back({TokenKind::LParen, "("});
                    advance();
                    break;

                case ')':
                    tokens.push_back({TokenKind::RParen, ")"});
                    advance();
                    break;

                case '[':
                    tokens.push_back({TokenKind::LBracket, "["});
                    advance();
                    break;

                case ']':
                    tokens.push_back({TokenKind::RBracket, "]"});
                    advance();
                    break;

                case '=':
                    tokens.push_back({TokenKind::Equal, "="});
                    advance();
                    break;

                case '+':
                    tokens.push_back({TokenKind::Plus, "+"});
                    advance();
                    break;

                case '-':
                    tokens.push_back({TokenKind::Minus, "-"});
                    advance();
                    break;

                case '*':
                    tokens.push_back({TokenKind::Star, "*"});
                    advance();
                    break;

                case '/':
                    tokens.push_back({TokenKind::Slash, "/"});
                    advance();
                    break;

                default:
                    std::cerr << "Unknown character: '" << c << "'\n";
                    advance();
                    break;
            }
        }

        tokens.push_back({TokenKind::Eof, ""});
        return tokens;
    }

private:
    std::string source;
    size_t pos = 0;

    bool atEnd() const {
        return pos >= source.size();
    }

    char peek() const {
        return atEnd() ? '\0' : source[pos];
    }

    char advance() {
        return source[pos++];
    }

    Token identifier() {
        size_t start = pos;

        while (!atEnd()) {
            char c = peek();

            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_')
                break;

            advance();
        }

        std::string text = source.substr(start, pos - start);

        if (text == "fn")
            return {TokenKind::Fn, text};

        if (text == "end")
            return {TokenKind::End, text};

        return {TokenKind::Identifier, text};
    }

    Token number() {
        size_t start = pos;

        while (!atEnd() &&
               std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }

        return {
            TokenKind::Number,
            source.substr(start, pos - start)
        };
    }
};