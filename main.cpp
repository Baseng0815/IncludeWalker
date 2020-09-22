#include <ncurses.h>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <iostream>
#include <filesystem>
#include <string_view>
#include <cstdio>

#include "interface.hpp"

int main(int argc, char **argv)
{
    Parser parser {"test/src/main.cpp"};
    Graph &graph = parser.getGraph();

    Interface interface {std::move(graph)};

    while (interface.isOpen()) {
        interface.draw();
        char c = getch();
        interface.charInput(c);
    }

    return 0;
}
