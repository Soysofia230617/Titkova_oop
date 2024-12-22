#ifndef BATTTLESIP_ABILITYMANAGER_H
#define BATTTLESIP_ABILITYMANAGER_H

#include <memory>
#include <queue>
#include <random>
#include "GameField.h"
#include "Interface.h"

const size_t NumAbilities = 3;

class AbilityManager {
public:
    AbilityManager(GameField& gamefield);
    ~AbilityManager() = default;

    std::queue<std::shared_ptr<Interface>> GetAbilities() const;

    void AddAbility(Abilities ability);
    void AddRandomAbility();
    void UseAbility();
    void Reset(GameField& gamefield);

private:
    GameField& gamefield_;
    std::queue<std::shared_ptr<Interface>> abqueue_;
};
#endif  // BATTTLESIP_ABILITYMANAGER_H
