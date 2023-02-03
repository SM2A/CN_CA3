#include <chrono>
//#include <thread>
//#include <future>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "Network.h"
#include <algorithm>

#define INF 99999
#define NO_LINK (-1)
#define SAME_NODE 0
#define NUM_WIDTH 5
#define SEPARATOR ' '
#define HOSTS "hosts"
#define ROUTERS "routers"
#define SAME_NODE_ERROR "Source and Destination cannot be the same"

using namespace std;

Network *Network::instance = nullptr;

Network *Network::getInstance() {
    if (instance == nullptr) instance = new Network();
    return instance;
}

void Network::draw() {

    ofstream file("graph.txt");
    file << nodes.size() << endl;
    for (auto &node : nodes) file << node->type << " ";
    file << endl;

    for (auto &node : nodes) file << node->ip << endl;

    cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << "      ";

    for (int i = 0; i < links.size(); ++i) cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1;
    cout << endl;

    for (int i = 0; i < ((links.size() * NUM_WIDTH) + NUM_WIDTH); ++i) cout << '_';
    cout << endl;

    for (int i = 0; i < links.size(); ++i) {
        cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << i + 1 << '|';
        for (auto j : links[i]) {
            cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << j.first;
            file << j.first << " ";
        }
        file << endl;
        cout << endl;
    }

    file.close();
    cout << endl;

    system("python3 Source/Routing/graph.py");
}

void Network::addLink(string source, string destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Node not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    for (int i = 0; links.size() <= max(srcNum, dstNum); ++i) {
        vector<pair<int, LinkStatus>> row;
        for (int j = 0; row.size() <= max(srcNum, dstNum); ++j) row.push_back(make_pair(NO_LINK, DOWN));
        links.push_back(row);
    }

    for (auto &link : links) for (int j = 0; link.size() <= max(srcNum, dstNum); ++j) link.push_back(make_pair(NO_LINK, DOWN));

    for (int i = 0; i < links.size(); ++i) {
        links[i][i].first = SAME_NODE;
        links[i][i].second = UP;
    }

    links[srcNum][dstNum].first = cost;
    links[srcNum][dstNum].second = UP;
    links[dstNum][srcNum].first = cost;
    links[dstNum][srcNum].second = UP;
    edges.emplace_back(make_pair(make_pair(srcNum, dstNum), cost), UP);
    edges.emplace_back(make_pair(make_pair(dstNum, srcNum), cost), UP);

    dvrp();
}

void Network::modifyLink(string source, string destination, int cost) {

    if (source == destination) {
        cerr << SAME_NODE_ERROR << endl;
        return;
    }

    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Node not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    links[srcNum][dstNum].first = cost;
    links[srcNum][dstNum].second = UP;
    links[dstNum][srcNum].first = cost;
    links[dstNum][srcNum].second = UP;

    bool sd = false, ds = false;

    for (auto i : edges) {
        if ((i.first.first.first == srcNum) && (i.first.first.second == dstNum)) {
            i.first.second = cost;
            sd = true;
        }
        if ((i.first.first.second == srcNum) && (i.first.first.first == dstNum)) {
            i.first.second = cost;
            ds = true;
        }
    }

    if (!sd && !ds) {
        edges.emplace_back(make_pair(make_pair(srcNum, dstNum), cost), UP);
        edges.emplace_back(make_pair(make_pair(dstNum, srcNum), cost), UP);
    }

    dvrp();
}

void Network::removeLink(string source, string destination) {

    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Node not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    links[srcNum][dstNum].first = NO_LINK;
    links[srcNum][dstNum].second = DOWN;
    links[dstNum][srcNum].first = NO_LINK;
    links[dstNum][srcNum].second = DOWN;

    for (int i = 0; i < edges.size(); ++i) {
        if ((edges[i].first.first.first == srcNum) && (edges[i].first.second == dstNum)) {
            edges.erase(edges.begin() + i);
            i--;
        }
        if ((edges[i].first.second == srcNum) && (edges[i].first.first.second == dstNum)) {
            edges.erase(edges.begin() + i);
            i--;
        }
    }

    dvrp();
}

void Network::dvrp(bool logTime) {
    auto begin = chrono::high_resolution_clock::now();

    for (int i = 0; i < links.size(); ++i) bellmanFord(i);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    if (logTime) cout << duration.count() << " Nanoseconds" << endl;
}

void Network::showTable(string src) {
    Node* node = findNode(src);

    if (node == nullptr) {
        cerr << "Node not found" << endl;
        return;
    }

    cout << "Destination                             Next Hop" << endl;
    cout << "---------------------------------------------------" << endl;
    for (auto entry: node->routingTable) {
        Node* dstNode = findNode(entry.first);
        Node* nextHop = findNode(entry.second);
        cout << dstNode->ip << "                            "
             << nextHop->ip << endl;
    }
    cout << "---------------------------------------------------" << endl << endl;
}

Node *Network::findNode(int num) {
    auto iter = find_if(nodes.begin(), nodes.end(), [num](Node *node) -> bool { return node->num == num; });
    if (iter != nodes.end()) return iter[0];
    else return nullptr;
}

Node *Network::findNode(string ip) {
    auto iter = find_if(nodes.begin(), nodes.end(), [ip](Node *node) -> bool { return node->ip == ip; });
    if (iter != nodes.end()) return iter[0];
    else return nullptr;
}

vector<int> Network::recvPrintPath(vector<int> const &parent, int vertex, int source, vector<int> path = {}) {
    if (vertex < 0) return path;
    if (parent[vertex] < 0) return path;
    if (vertex == source)  return path;
    path.push_back(vertex);
    return recvPrintPath(parent, parent[vertex], source, path);
}

void Network::bellmanFord(int src) {

    Node *node = findNode(src);
    node->routingTable.clear();
    node->clear();
    node->init(links.size());

    node->distance[src] = SAME_NODE;

    for (int i = 0; i < links.size(); i++) {
        for (auto &edge : edges) {
            int u = edge.first.first.first;
            int v = edge.first.first.second;
            if (edgeStatus(u, v) == DOWN) continue;
            int cost = edge.first.second;
            if (((node->distance[u] + cost) < node->distance[v]) && (node->distance[u] != INF)) {
                node->path[v] = u;
                node->distance[v] = node->distance[u] + cost;
            }
        }
    }

    for (int k = 0; k < links.size(); k++) {
        if (src == k) continue;
        if (findNode(k)->type == ROUTER) continue;
        vector<int> path = recvPrintPath(node->path, k, src, vector<int>());
        if (path.size() == 1) node->routingTable.push_back(make_pair(k, path[0]));
        else if (path.size() > 1) node->routingTable.push_back(make_pair(k, path[1]));
    }

    node->clear();
}

void Network::addNode(string type, string ip) {
    Type nodeType;
    if (type == HOSTS) nodeType = HOST;
    else if (type == ROUTERS) nodeType = ROUTER;
    else {
        cerr << "Node type error" << endl;
        return;
    }
    nodes.push_back(new Node(lastNodeNum, nodeType, ip));
    lastNodeNum++;
}

void Network::linkStatus(string source, string destination) {
    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Link not found" << endl;
        return;
    }

    if (links[srcNode->num][dstNode->num].second == UP) cout << "UP" << endl;
    else cout << "DOWN" << endl;
}

/*void makeDown(int time, int src, int dst) {
    this_thread::sleep_for(chrono::seconds(time));
    Network::getInstance()->links[src][dst].second = DOWN;
    Network::getInstance()->links[dst][src].second = DOWN;

    Network::getInstance()->changeEdgeStatus(src, dst, DOWN);
}

void bringUp(int time, int src, int dst) {
    this_thread::sleep_for(chrono::seconds(time));
    Network::getInstance()->links[src][dst].second = UP;
    Network::getInstance()->links[dst][src].second = UP;

    Network::getInstance()->changeEdgeStatus(src, dst, UP);
}*/

void Network::downLink(string source, string destination, int start, int duration) {
    Node* srcNode = findNode(source);
    Node* dstNode = findNode(destination);

    if ((srcNode == nullptr) || (dstNode == nullptr)) {
        cerr << "Link not found" << endl;
        return;
    }

    int srcNum = srcNode->num;
    int dstNum = dstNode->num;

    /*future<void> down = async(makeDown, start, srcNum, dstNum);
    future<void> up = async(bringUp, duration, srcNum, dstNum);*/

    links[srcNum][dstNum].second = DOWN;
    links[dstNum][srcNum].second = DOWN;

    changeEdgeStatus(srcNum, dstNum, DOWN);

    dvrp();
}

LinkStatus Network::edgeStatus(int src, int dst) {
    for (auto &edge: edges) if ((edge.first.first.first == src) && (edge.first.first.second == dst)) return edge.second;
    return DOWN;
}

void Network::changeEdgeStatus(int src, int dst, LinkStatus status) {
    for (auto &edge: edges) {
        if (((edge.first.first.first == src) && (edge.first.first.second == dst))
        || ((edge.first.first.first == dst) && (edge.first.first.second == src))) {
            edge.second = status;
        }
    }
}
