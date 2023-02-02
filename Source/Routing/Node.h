#ifndef NODE_H
#define NODE_H

#include <vector>

class Node {

private:

public:

    int num;

    std::vector<int> distance;
    std::vector<int> prev;
    std::vector<int> path;

    Node(int num) : num(num) {}

    void init(int count);
    void clear();
};

#endif
