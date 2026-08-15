#pragma once

#include <string>
#include <vector>

struct Node {
    enum class Kind {
        Number,
        Float,
        Name,
        Array,
        Binary,
        Call,
        Index
    };

    Kind kind;

    std::string text;
    std::vector<Node> children;
};

struct Statement {
    enum class Kind {
        Assignment,
        Expression
    };

    Kind kind;

    Node target; // Name or Index for Assignment
    Node value;  // RHS or expression
};

struct Function {
    std::string name;
    std::vector<Statement> body;
};

struct Program {
    std::vector<Function> functions;
};