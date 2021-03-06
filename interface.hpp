#include "graph.hpp"

#include <stack>

struct InterfaceNode {
    NodeRef node;
    bool isCollapsed;
    int level;
};

struct HistoryEntry {
    NodeRef node;
    int cursorPos;
};

class Interface {
    private:
        std::stack<HistoryEntry> m_history;
        std::vector<InterfaceNode> m_interfaceNodes;
        Graph m_graph;

        bool m_isOpen = true;
        int m_cursorPos = 0;

        void recalculateInterfaceNodes(NodeRef root);
        void addInterfaceNodes(NodeRef start, int level = 0);

        // returns -1 if not found
        int prevInterfaceNodeIndex() const;
        int nextInterfaceNodeIndex() const;

    public:
        Interface(Graph &&graph);
        ~Interface();

        void charInput(char c);

        bool isOpen() const;

        void draw();
};
