#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include "Network.h"

#define ADD "add"
#define UPDATE "update"
#define LINK "link"
#define HOSTS "hosts"
#define ROUTERS "routers"
#define LOG_STATUS "log status"
#define RUN "run"
#define DRAW "draw"
#define REMOVE "remove"
#define SHOW_TABLE "show"

class CommandParser {

private:

    static CommandParser *instance;
    Network *network;

    CommandParser() {
        network = Network::getInstance();
    }

    void parseAdd(const std::string &data);
    void parseUpdate(const std::string &data);
    void parseRemove(const std::string &data);
    void parseDraw();
    void parseRun();
    void parseLog(const std::string &data);
    void parseShow(const std::string &data);

public:

    static CommandParser *getInstance();

    void parse(const std::string &command);

};

#endif
