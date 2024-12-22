#include "CommandProvider.h"

CommandProvider::CommandProvider(const std::string& filename) {
    if (!LoadFromFile(filename)) {
        SetDefaultCommands();
    }
}

GameCommand CommandProvider::GetCommand() {
    std::cout << "Enter command key: ";
    char ch;
    std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = key_to_command_.find(ch);
    if (it != key_to_command_.end()) {
        return it->second;
    }
    return GameCommand::UNKNOWN;
}

bool CommandProvider::GetCoordinates(int& x, int& y) {
    std::cout << "Enter coordinates (x y): ";
    std::cin >> x >> y;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid coordinates input!" << std::endl;
        return false;
    }
    x--; y--;
    return true;
}

bool CommandProvider::LoadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    std::unordered_map<std::string, GameCommand> str_to_command = {
            {"USE_ABILITY", GameCommand::USE_ABILITY},
            {"DEAL_DAMAGE", GameCommand::DEAL_DAMAGE},
            {"SAVE_GAME",   GameCommand::SAVE_GAME},
            {"LOAD_GAME",   GameCommand::LOAD_GAME},
            {"EXIT",        GameCommand::EXIT},
            {"ATTACK",      GameCommand::ATTACK}
    };

    char key;
    std::string cmd_str;
    while (in >> key >> cmd_str) {
        auto it = str_to_command.find(cmd_str);
        if (it != str_to_command.end()) {
            key_to_command_[key] = it->second;
        } else {
            return false;
        }
    }

    return !key_to_command_.empty();
}

void CommandProvider::SetDefaultCommands() {
    key_to_command_.clear();
    key_to_command_['U'] = GameCommand::USE_ABILITY;
    key_to_command_['D'] = GameCommand::DEAL_DAMAGE;
    key_to_command_['S'] = GameCommand::SAVE_GAME;
    key_to_command_['L'] = GameCommand::LOAD_GAME;
    key_to_command_['Q'] = GameCommand::EXIT;
}
