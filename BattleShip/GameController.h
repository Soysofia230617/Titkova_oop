#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include "GameDisplay.h"
#include "ConsoleRender.h"
#include "GameDisplay.h"

#include "GameManager.h"
#include "Commands.h"
#include <iostream>

template<typename CommandProvider>
class GameController {
public:
    GameController(GameManager &gameManager, CommandProvider &inputHandler, GameDisplay<ConsoleRenderer> &display)
            : gameManager_(gameManager), inputHandler_(inputHandler), display_(display) {}

    void Run() {
        bool running = true;
        while (running) {
            GameCommand command = inputHandler_.GetCommand();
            ExecuteCommand(command, running);
            gameManager_.EnemyTurn();
            display_.Update(gameManager_);
            if (gameManager_.CheckGameOver()) {
                running = false;
            }
        }
    }

private:
    GameManager& gameManager_;
    CommandProvider& inputHandler_;
    GameDisplay<ConsoleRenderer>& display_;

    void ExecuteCommand(GameCommand command, bool& running) {
        switch (command) {
            case GameCommand::USE_ABILITY:
                gameManager_.UseAbility();
                break;
            case GameCommand::DEAL_DAMAGE: {
                int x, y;
                if (inputHandler_.GetCoordinates(x, y)) {
                    try {
                        gameManager_.DealDamageToEnemy(x, y);
                    } catch (OutOfBounds&) {
                        std::cout << "Attack out of bounds!" << std::endl;
                    }
                }
            } break;
            case GameCommand::SAVE_GAME: {
                std::string filename;
                std::cout << "Enter filename to save: ";
                std::cin >> filename;
                gameManager_.SaveToFile(filename);
            } break;
            case GameCommand::LOAD_GAME: {
                std::string filename;
                std::cout << "Enter filename to load: ";
                std::cin >> filename;
                gameManager_.LoadFromFile(filename);
            } break;
            case GameCommand::EXIT:
                running = false;
                break;
            default:
                std::cout << "Unknown command!" << std::endl;
                break;
        }
    }
};

#endif // GAMECONTROLLER_H
