#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include "Network.h"

#define TOPOLOGY "topology"
#define SHOW "show"
#define LSRP "lsrp"
#define DVRP "dvrp"
#define MODIFY "modify"
#define REMOVE "remove"

class CommandParser {

private:

    static CommandParser *instance;
    Network *network;

    CommandParser() {
        network = Network::getInstance();
    }

    void parseTopology(const std::string &data);
    void parseModify(const std::string &data);
    void parseRemove(const std::string &data);
    void parseShow();
    void parseLSRP(const std::string &data);
    void parseDVRP(const std::string &data);

public:

    static CommandParser *getInstance();

    void parse(const std::string &command);

};

#endif
