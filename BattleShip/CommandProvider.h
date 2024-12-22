#ifndef COMMANDPROVIDER_H
#define COMMANDPROVIDER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include "Commands.h"

class CommandProvider {
public:
    CommandProvider(const std::string& filename = "commands.txt");

    GameCommand GetCommand();
    bool GetCoordinates(int& x, int& y);

private:
    std::unordered_map<char, GameCommand> key_to_command_;

    bool LoadFromFile(const std::string& filename);
    void SetDefaultCommands();
};

#endif // COMMANDPROVIDER_H
