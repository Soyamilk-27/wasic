#pragma once

#include <iostream>

#include "types/types_parser.h"

inline void dumpNode(const Node& node, int indent = 0) {
    const std::string pad(indent, ' ');

    switch (node.kind) {
        case Node::Kind::Number:
            std::cout << pad << "Number: " << node.text << '\n';
            break;

        case Node::Kind::Name:
            std::cout << pad << "Name: " << node.text << '\n';
            break;

        case Node::Kind::Array:
            std::cout << pad << "Array\n";

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;

        case Node::Kind::Binary:
            std::cout << pad << "Binary: " << node.text << '\n';

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;

        case Node::Kind::Call:
            std::cout << pad << "Call: " << node.text << '\n';

            for (const auto& child : node.children)
                dumpNode(child, indent + 2);

            break;
    }
}

inline void dumpProgram(const Program& program) {
    std::cout << "Program\n";

    for (const auto& fn : program.functions) {
        std::cout << "  Function: " << fn.name << '\n';

        for (const auto& stmt : fn.body) {
            if (stmt.kind == Statement::Kind::Assignment) {
                std::cout
                    << "    Assignment: "
                    << stmt.name
                    << '\n';

                dumpNode(stmt.value, 6);
            }
            else {
                std::cout << "    Expression\n";
                dumpNode(stmt.value, 6);
            }
        }
    }
}