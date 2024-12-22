#include "AbilityManager.h"

AbilityManager::AbilityManager(GameField& gamefield) : gamefield_(gamefield) {
    std::vector<Abilities> abilities = {
            Abilities::DOUBLEDAMAGE,
            Abilities::SCAN,
            Abilities::BOMBING
    };

    std::random_device rd;
    std::mt19937 random_generator(rd());

    std::shuffle(abilities.begin(), abilities.end(), random_generator);

    for (auto ability : abilities) {
        AddAbility(ability);
    }
}

std::queue<std::shared_ptr<Interface>> AbilityManager::GetAbilities() const {
    return abqueue_;
}

void AbilityManager::AddAbility(Abilities ability) {
    switch (ability) {
        case Abilities::DOUBLEDAMAGE: {
            auto double_damage = std::make_shared<DoubleDamage>();
            abqueue_.push(double_damage);
        } break;
        case Abilities::SCAN: {
            auto scan = std::make_shared<Scan>();
            abqueue_.push(scan);
        } break;
        case Abilities::BOMBING: {
            auto bombing = std::make_shared<Bombing>();
            abqueue_.push(bombing);
        } break;
        default:
            throw NoSuchAbility();
    }
}

void AbilityManager::AddRandomAbility() {
    Abilities ability = static_cast<Abilities>( rand() % NumAbilities);
    AddAbility(ability);
}

void AbilityManager::UseAbility() {
    if (abqueue_.empty()) {
        throw NoAbility();
    }
    abqueue_.front()->UseAbility(gamefield_);
    abqueue_.pop();
}

void AbilityManager::Reset(GameField& gamefield) {
    gamefield_ = gamefield;
    abqueue_ = std::queue<std::shared_ptr<Interface>>();
}
