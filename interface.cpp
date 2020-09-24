#include "interface.hpp"

#include <ncurses.h>

void Interface::recalculateInterfaceNodes(NodeRef root)
{
    m_interfaceNodes.clear();

    addInterfaceNodes(root);
    clear();
}

void Interface::addInterfaceNodes(NodeRef start, int level)
{
    m_interfaceNodes.emplace_back(InterfaceNode {start, false, level});

    if (start->nodeType == NodeType::SYSTEM) {
        return;
    }

    for (NodeRef child : start->children) {
        addInterfaceNodes(child, level + 1);
    }
}

int Interface::prevInterfaceNodeIndex() const
{
    for (int i = m_cursorPos - 1; i >= 0; i--) {
        if (m_interfaceNodes[i].level == m_interfaceNodes[m_cursorPos].level) {
            return i;
        }
    }

    return -1;
}

int Interface::nextInterfaceNodeIndex() const
{
    for (int i = m_cursorPos + 1; i < m_interfaceNodes.size(); i++) {
        if (m_interfaceNodes[i].level == m_interfaceNodes[m_cursorPos].level) {
            return i;
        }
    }

    return -1;
}

Interface::Interface(Graph &&graph)
    : m_graph {std::move(graph)}
{
    initscr();
    curs_set(0);
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    move(0, 0);

    recalculateInterfaceNodes(m_graph.nodes.back().get());
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
        case 'J':
            {
                int nextNode = nextInterfaceNodeIndex();
                if (nextNode != -1) {
                    m_cursorPos = nextNode;
                }
            }
            break;
        case 'K':
            {
                int prevNode = prevInterfaceNodeIndex();
                if (prevNode != -1) {
                    m_cursorPos = prevNode;
                }
            }
            break;
        case 'k':
            if (m_cursorPos > 0) {
                m_cursorPos--;
            }
            break;
        case 'l':
            {
                NodeRef n = m_interfaceNodes[m_cursorPos].node;
                if (n->nodeType == NodeType::LOCAL) {
                    m_history.push(HistoryEntry {m_interfaceNodes.front().node, m_cursorPos});
                    recalculateInterfaceNodes(n);
                    m_cursorPos = 0;
                }
            }
            break;

        case 'h':
            if (m_history.size() > 0) {
                recalculateInterfaceNodes(m_history.top().node);
                m_cursorPos = m_history.top().cursorPos;
                m_history.pop();
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

            mvaddstr(y++, iNode.level * 2, iNode.node->nodeFile.c_str());
        }
    }
}
