#include "Node.h"

#define INF 99999
#define NO_LINK -1

using namespace std;

void Node::init(int count) {
    for (int i = 0; i <count ; ++i) {
        prev.push_back(NO_LINK);
        distance.push_back(INF);
        path.push_back(NO_LINK);
    }
}

void Node::clear() {
    prev.clear();
    path.clear();
    distance.clear();
}
