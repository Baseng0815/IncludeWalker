#include "interface.hpp"

#include <ncurses.h>

void Interface::recalculateInterfaceNodes(std::string_view start)
{
    m_interfaceNodes.clear();

    addInterfaceNodes(m_graph.getNodeByFile(start));
}

void Interface::addInterfaceNodes(const Node *start, int level)
{
    m_interfaceNodes.emplace_back(InterfaceNode {start, false, level});

    if (start->nodeType == NodeType::SYSTEM) {
        return;
    }

    for (const Node *child : start->children) {
        addInterfaceNodes(child, level + 1);
    }
}

Interface::Interface(Graph &&graph)
    : m_graph {std::move(graph)}
{
    initscr();
    curs_set(1);
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    move(0, 0);

    recalculateInterfaceNodes(m_graph.nodes.back()->nodeFile);
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
        case 'j':
            if (m_cursorPos < m_interfaceNodes.size() - 1) {
                m_cursorPos++;
            }
            break;
        case 'k':
            if (m_cursorPos > 0) {
                m_cursorPos--;
            }
            break;
    }
}

bool Interface::isOpen() const
{ return m_isOpen; }

void Interface::draw()
{
    int y = 0;
    for (const auto &iNode : m_interfaceNodes) {
        if (!iNode.isCollapsed) {
            if (y == m_cursorPos) {
                color_set(1, NULL);
            } else {
                color_set(0, NULL);
            }

            mvaddstr(y++, iNode.level * 2, iNode.graphNode->nodeFile.c_str());
        }
    }
}
