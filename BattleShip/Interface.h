#ifndef BATTTLESIP_INTERFACE_H
#define BATTTLESIP_INTERFACE_H

#include "GameField.h"
#include "Enum.h"

class Interface {
public:
    Interface(Abilities type) : type_(type) {}
    Abilities GetType() const;
    void UseAbility(GameField& gamefield);

private:
    virtual void Ability_(GameField& gamefield) = 0;
    Abilities type_;
};

class DoubleDamage : public Interface {
public:
    DoubleDamage(): Interface(Abilities::DOUBLEDAMAGE) { }

private:
    void Ability_(GameField& gamefield) override;
};

class Scan : public Interface {
public:
    Scan() : Interface(Abilities::SCAN) {}

private:
    void Ability_(GameField& gamefield) override;
};

class Bombing : public Interface {
public:
    Bombing() : Interface(Abilities::BOMBING) {}

private:
    void Ability_(GameField& gamefield) override;
};

#endif //BATTTLESIP_INTERFACE_H
