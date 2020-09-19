#include "interface.hpp"

#include <ncurses.h>

void Interface::recalculateInterfaceNodes()
{
    m_interfaceNodes.clear();

    for (const auto &node : m_graph.nodes) {
        m_interfaceNodes.emplace_back(node, false);
    }
}

Interface::Interface(Graph graph)
    : m_graph {graph}
{
    initscr();
    curs_set(1);
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    color_set(1, 0);

    recalculateInterfaceNodes();
}

Interface::~Interface()
{
    endwin();
}

void Interface::charInput(char c)
{
    switch (c) {
        case 'q':
            m_isOpen = false;
            break;
    }
}

bool Interface::isOpen() const
{ return m_isOpen; }

void Interface::draw()
{
    for (const auto &iNode : m_interfaceNodes) {
        if (!iNode.isCollapsed) {

        }
    }
}
