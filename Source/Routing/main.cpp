#include <iostream>
#include "CommandParser.h"

using namespace std;

int main() {

    string input;
    while (getline(cin, input)) CommandParser::getInstance()->parse(input);

    return 0;
}
