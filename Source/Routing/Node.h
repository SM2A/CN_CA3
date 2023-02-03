#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

enum Type { HOST, ROUTER };

class Node {

private:

public:

    int num;
    Type type;
    std::string ip;

    std::vector<int> distance;
    std::vector<int> prev;
    std::vector<int> path;

    Node(int num, Type type, std::string ip) : num(num), type(type), ip(ip) {}

    void init(int count);
    void clear();
};

#endif
