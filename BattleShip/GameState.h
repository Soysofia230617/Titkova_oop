#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <fstream>
#include <iostream>
#include <vector>
#include "AbilityManager.h"
#include "Enum.h"
#include "GameField.h"

class GameState {
public:
    GameState(const GameField& player_field, const GameField& enemy_field,
              const std::queue<Abilities>& abilities, size_t prev_destroyed)
            : player_field_(player_field),
              enemy_field_(enemy_field),
              abilities_(abilities),
              prev_destroyed_(prev_destroyed) {}

    GameField GetPlayerField();
    GameField GetEnemyField();
    std::queue<Abilities> GetAbilities();
    size_t GetPrevDestroyed();

    void SaveToStream(std::ostream& out) const;
    void LoadFromStream(std::istream& in);

    std::ostream& operator<<(std::ostream& out);
    std::istream& operator>>(std::istream& in);

private:
    GameField player_field_;
    GameField enemy_field_;
    std::queue<Abilities> abilities_;
    size_t prev_destroyed_;
};

#endif  // GAMESTATE_H
