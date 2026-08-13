#pragma once
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "../types/types_parser.h"

class Parser {
public:
    explicit Parser(const std::vector<Token> tokens)
        : source(std::move(tokens)) {}

    Program parse() {
        for (const auto& token : source) {

        }
    }

private:
    std::string source;

    void identityFunction() {

    }
};
