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
    /*
    Parser parser {"test/src/main.cpp"};
    Graph graph = parser.getGraph();
    */

    Interface interface {Graph {}};

    while (interface.isOpen()) {
        char c = getch();
        mvprintw(1, 0, "%c\n", c);
    }

    return 0;

    /* color_set(0, 0); */
    /* move(curline, 0); */

    /* switch (c) { */
    /*     case 'q': */
    /*         running = false; */
    /*         break; */
    /*     case 'a': */
    /*         add_string(lines); */
    /*         break; */
    /*     case 'j': */
    /*         if (curline < lines.size() - 1) curline++; */
    /*         break; */
    /*     case 'k': */
    /*         if (curline > 0) curline--; */
    /*         break; */
    /* } */
}
