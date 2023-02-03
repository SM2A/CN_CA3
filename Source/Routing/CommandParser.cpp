#include <sstream>
#include "CommandParser.h"

#define SEPARATOR ' '

using namespace std;

CommandParser *CommandParser::instance = nullptr;

CommandParser *CommandParser::getInstance() {
    if (instance == nullptr) instance = new CommandParser();
    return instance;
}

void CommandParser::parse(const string &command) {
    string cmd;
    stringstream stream(command);
    getline(stream, cmd, SEPARATOR);

    if (cmd == ADD) parseAdd(stream.str());
    else if (cmd == UPDATE) parseUpdate(stream.str());
    else if (cmd == REMOVE) parseRemove(stream.str());
    else if (cmd == LOG_STATUS) parseLog(stream.str());
    else if (cmd == RUN) parseRun();
    else if (cmd == DRAW) parseDraw();
    else if (cmd == SHOW_TABLE) parseShow(stream.str());
}

void CommandParser::parseRemove(const string &data) {
    string temp, branch;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);
    getline(stream, branch, SEPARATOR);

    string source, destination, cost;

    getline(stream, source, SEPARATOR);
    getline(stream, destination, SEPARATOR);
    getline(stream, cost, SEPARATOR);

    network->removeLink(source, destination);
}

void CommandParser::parseAdd(const string &data) {
    string temp, branch;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);
    getline(stream, branch, SEPARATOR);

    if ((branch == HOSTS) || (branch == ROUTERS)) {
        string ip;
        while (getline(stream, ip, SEPARATOR)) network->addNode(branch, ip);
    } else if (branch == LINK) {
        string value;
        while (getline(stream, value)) {
            stringstream entry(value);
            string source, destination, cost;

            getline(entry, source, SEPARATOR);
            getline(entry, destination, SEPARATOR);
            getline(entry, cost, SEPARATOR);

            network->addLink(source, destination, stoi(cost));
        }
    }
}

void CommandParser::parseUpdate(const string &data) {
    string temp, branch;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);
    getline(stream, branch, SEPARATOR);

    if (branch == LINK) {
        string source, destination, cost;

        getline(stream, source, SEPARATOR);
        getline(stream, destination, SEPARATOR);
        getline(stream, cost, SEPARATOR);

        network->modifyLink(source, destination, stoi(cost));
    }
}

void CommandParser::parseDraw() {
    network->draw();
}

void CommandParser::parseRun() {
    network->dvrp();
}

void CommandParser::parseLog(const string &data) {
    string temp1, temp2;
    stringstream stream(data);
    getline(stream, temp1, SEPARATOR);
    getline(stream, temp2, SEPARATOR);

    string source, destination;

    getline(stream, source, SEPARATOR);
    getline(stream, destination, SEPARATOR);

    network->linkStatus(source, destination);
}

void CommandParser::parseShow(const string &data) {
    string ip, temp1, temp2;
    stringstream stream(data);
    getline(stream, temp1, SEPARATOR);
    getline(stream, temp2, SEPARATOR);

    getline(stream, ip);

    network->showTable(ip);
}
