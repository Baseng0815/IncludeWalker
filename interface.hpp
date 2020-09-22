#include "graph.hpp"

struct InterfaceNode {
    const Node *graphNode;
    bool isCollapsed;
    int level;
};

class Interface {
    private:
        std::vector<InterfaceNode> m_interfaceNodes;
        Graph m_graph;

        bool m_isOpen = true;
        int m_cursorPos = 0;

        void recalculateInterfaceNodes(std::string_view start);
        void addInterfaceNodes(const Node *start, int level = 0);

    public:
        Interface(Graph&& graph);
        ~Interface();

        void charInput(char c);

        bool isOpen() const;

        void draw();
};
