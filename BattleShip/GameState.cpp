#include "GameState.h"

void GameState::SaveToStream(std::ostream& out) const {
    out << prev_destroyed_ << std::endl;

    player_field_.SaveToStream(out);
    enemy_field_.SaveToStream(out);

    std::queue<Abilities> temp_abilities(abilities_);

    out << temp_abilities.size() << std::endl;
    while (!temp_abilities.empty()) {
        out << static_cast<int32_t>(temp_abilities.front()) << " ";
        temp_abilities.pop();
    }

    out << std::endl;
}

void GameState::LoadFromStream(std::istream& in) {
    in >> prev_destroyed_;

    player_field_.LoadFromStream(in);
    enemy_field_.LoadFromStream(in);

    size_t ability_count;
    in >> ability_count;
    abilities_ = std::queue<Abilities>();

    for (size_t i = 0; i < ability_count; ++i) {
        int32_t ability;
        in >> ability;
        abilities_.push(static_cast<Abilities>(ability));
    }
}

GameField GameState::GetPlayerField() { return player_field_; }
GameField GameState::GetEnemyField() { return enemy_field_; }
std::queue<Abilities> GameState::GetAbilities() { return abilities_; }
size_t GameState::GetPrevDestroyed() { return prev_destroyed_; }

std::ostream& GameState::operator<<(std::ostream& out) {
    SaveToStream(out);
    return out;
}

std::istream& GameState::operator>>(std::istream& in) {
    LoadFromStream(in);
    return in;
}
