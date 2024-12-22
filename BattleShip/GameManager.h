#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <queue>
#include "AbilityManager.h"
#include "HashSum.h"
#include "GameField.h"

class GameManager {
public:
    GameManager();

    void Init();

    void EnemyTurn();
    void UseAbility();
    bool CheckGameOver();
    void RestartGame();
    void StartNextRound();
    void SaveToFile(std::string& filename) const;
    void LoadFromFile(std::string& filename);

    void DisplayFields_() const;

    GameField enemy_field_;
    void DealDamageToEnemy(int x, int y);

    void DisplayAbilities_() const;

private:
    bool GetCoordinates_(int& x, int& y);
    void DisplayField_(const GameField& field) const;

    void InitShips_();
    void SaveToStream_(std::ostream& out) const;
    void LoadFromStream_(std::istream& in);
    void InitFields_();

    GameField player_field_;
    AbilityManager ability_mgr_;
    size_t prev_destroyed_;
    bool is_player_turn_;
};

#endif // GAMEMANAGER_H
