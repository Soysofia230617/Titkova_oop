#ifndef BATTTLESIP_GAMEFIELD_H
#define BATTTLESIP_GAMEFIELD_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>

#include "Enum.h"
#include "Expectation.h"
#include "Ship.h"
#include "ShipManager.h"

enum class FieldCellStates { UNKNOWN = 0, WATER = 1, SHIP = 2, DESTROYD = 3 };

class GameField {
public:
    GameField();
    GameField(std::pair<uint32_t, uint32_t> size)
            : field_(size.first, std::vector<FieldCellStates>(size.second, FieldCellStates::UNKNOWN)),
              size_(size),
              shipmanager_(0, std::vector<size_t>(0)),
              ability_type_(Abilities::UNDEFINED) {}

    GameField(const GameField& gamefield);
    GameField(GameField&& other) noexcept;

    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    ~GameField() = default;

    void AddShip(Ship ship);
    void SetShipMgr(size_t cship, std::vector<size_t> sizes);
    void SetAbility(Abilities ability_type);
    void Recover();
    void InitEnemyShips(size_t pl_shipc, std::vector<size_t> sizes);
    bool is_free_cell(int x, int y, std::vector<std::pair<int, int>>& cells);

    size_t GetNumDestroyed() const;
    size_t GetNumShips() const;
    Abilities GetCurAbility() const;
    std::vector<std::vector<FieldCellStates>> GetField() const;

    void SaveToStream(std::ostream& out) const;
    void LoadFromStream(std::istream& in);
    int GetWidth() const;
    void DealDamage(std::pair<uint32_t, uint32_t> position);

private:
    std::vector<std::vector<FieldCellStates>> field_;
    std::pair<uint32_t, uint32_t> size_;
    ShipManager shipmanager_;
    Abilities ability_type_;

    void DealDamage_(std::pair<uint32_t, uint32_t> position, uint32_t damage,bool silent = false);
    void DoubleDamage_(std::pair<uint32_t, uint32_t> position);
    void Bombing_();
    void Scan_(std::pair<uint32_t, uint32_t> position);
};

#endif  // BATTTLESIP_GAMEFIELD_H
