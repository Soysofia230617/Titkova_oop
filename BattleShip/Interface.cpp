#include "Interface.h"

Abilities Interface::GetType() const { return type_; }

void Interface::UseAbility(GameField& gamefield) { Ability_(gamefield); }

void DoubleDamage::Ability_(GameField& gamefield) {
    gamefield.SetAbility(Abilities::DOUBLEDAMAGE);
}

void Scan::Ability_(GameField& gamefield) {
    gamefield.SetAbility(Abilities::SCAN);
}

void Bombing::Ability_(GameField& gamefield) {
    gamefield.SetAbility(Abilities::BOMBING);
}
