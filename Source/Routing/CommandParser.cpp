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
    string value, temp;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);
    getline(stream, temp, SEPARATOR);

    string source, destination;

    getline(stream, source, SEPARATOR);
    getline(stream, destination, SEPARATOR);

    network->removeLink(stoi(source), stoi(destination));
}

void CommandParser::parseAdd(const string &data) {
    string value, temp;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);

    while (getline(stream, value, SEPARATOR)) {
        stringstream entry(value);
        string source, destination, cost;

        getline(entry, source, DELIMITER);
        getline(entry, destination, DELIMITER);
        getline(entry, cost, DELIMITER);

        network->addLink(stoi(source), stoi(destination), stoi(cost));
    }
}

void CommandParser::parseUpdate(const string &data) {
    string value, temp;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);

    string source, destination, cost;

    getline(stream, source, DELIMITER);
    getline(stream, destination, DELIMITER);
    getline(stream, cost, DELIMITER);

    network->modifyLink(stoi(source), stoi(destination), stoi(cost));
}

void CommandParser::parseDraw() {
    network->draw();
}

void CommandParser::parseRun() {
    network->dvrp();
}

void CommandParser::parseLog(const string &data) {

}

void CommandParser::parseShow(const string &data) {
    string value, temp;
    stringstream stream(data);
    getline(stream, temp, SEPARATOR);

    string node;
    getline(stream, node);

    network->dvrp(stoi(node));
}
