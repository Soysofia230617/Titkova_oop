#include "GameField.h"

GameField::GameField() : field_({}), size_({0, 0}), shipmanager_(ShipManager(0, {})) {}

GameField::GameField(const GameField& gamefield)
        : field_(gamefield.field_),
          size_(gamefield.size_),
          shipmanager_(gamefield.shipmanager_) {}

GameField::GameField(GameField&& other) noexcept
        : field_(std::move(other.field_)),
          size_(other.size_),
          shipmanager_(std::move(other.shipmanager_)) {
    other.size_ = {0, 0};
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        field_ = other.field_;
        size_ = other.size_;
        shipmanager_ = other.shipmanager_;
        ability_type_ = other.ability_type_;
    }

    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        field_ = std::move(other.field_);
        size_ = other.size_;
        shipmanager_ = std::move(other.shipmanager_);
        ability_type_ = other.ability_type_;
        other.size_ = {0, 0};
    }
    return *this;
}

void GameField::AddShip(Ship new_ship) {
    std::pair<uint32_t, uint32_t> offset;
    if (new_ship.GetOrientation() == ShipOrientation::HORIZONTAL) {
        offset = {new_ship.GetSize() - 1, 0};
    } else {
        offset = {0, new_ship.GetSize() - 1};
    }

    if (new_ship.GetPosition().first + offset.first >= size_.first ||
        new_ship.GetPosition().second + offset.second >= size_.second) {
        throw OutOfBounds();
    }

    shipmanager_.AddShip(new_ship);
}

void GameField::DealDamage(std::pair<uint32_t, uint32_t> position) {
    switch (ability_type_) {
        case Abilities::DOUBLEDAMAGE:
            DoubleDamage_(position);
            break;
        case Abilities::SCAN:
            Scan_(position);
            break;
        case Abilities::BOMBING:
            Bombing_();
            break;
        case Abilities::UNDEFINED:
            DealDamage_(position, 1);
            break;
        default:
            break;
    }

    ability_type_ = Abilities::UNDEFINED;
}

void GameField::SetShipMgr(size_t cship, std::vector<size_t> sizes) {
    if (cship != sizes.size()) {
        throw SizeMismatch();
    }

    for (size_t size : sizes) {
        if (size == 0 || size > 4) {
            throw NoSuchShipSize();
        }
    }

    shipmanager_ = ShipManager(cship, sizes);
}

void GameField::SetAbility(Abilities ability_type) {
    ability_type_ = ability_type;
}

void GameField::Recover() {
    field_.clear();
    field_.resize(size_.second);
    for (uint32_t i = 0; i < size_.second; ++i) {
        field_[i].resize(size_.first, FieldCellStates::UNKNOWN);
    }
}

size_t GameField::GetNumDestroyed() const {
    size_t destroyed = 0;
    for (Ship& ship : shipmanager_.GetShips()) {
        if (ship.IsDestroyed()) {
            ++destroyed;
        }
    }

    return destroyed;
}

size_t GameField::GetNumShips() const { return shipmanager_.GetNumShips(); }

Abilities GameField::GetCurAbility() const { return ability_type_; }

std::vector<std::vector<FieldCellStates>> GameField::GetField() const {
    return field_;
}

void GameField::SaveToStream(std::ostream& out) const {
    out << size_.first << " " << size_.second << std::endl;
    for (uint32_t i = 0; i < size_.second; ++i) {
        for (uint32_t j = 0; j < size_.first; ++j) {
            out << static_cast<int32_t>(field_[i][j]) << " ";
        }
        out << std::endl;
    }
    shipmanager_.SaveToStream(out);
}

void GameField::LoadFromStream(std::istream& in) {
    in >> size_.first >> size_.second;
    field_.clear();
    field_.resize(size_.second);
    for (uint32_t i = 0; i < size_.second; ++i) {
        field_[i].resize(size_.first);
        for (uint32_t j = 0; j < size_.first; ++j) {
            int32_t state;
            in >> state;
            field_[i][j] = static_cast<FieldCellStates>(state);
        }
    }
    shipmanager_.LoadFromStream(in);
}

void GameField::DealDamage_(std::pair<uint32_t, uint32_t> position,
                            uint32_t damage, bool silent) {
    if (position.first >= size_.first || position.second >= size_.second) {
        throw OutOfBounds();
    }

    CellStates state = shipmanager_.DealDamage(position, damage);

    if (silent) {
        return;
    }

    if (state == CellStates::DESTROYED) {
        field_[position.second][position.first] = FieldCellStates::DESTROYD;
    } else if (state == CellStates::WATER) {
        field_[position.second][position.first] = FieldCellStates::WATER;
    } else {
        field_[position.second][position.first] = FieldCellStates::SHIP;
    }
}

void GameField::DoubleDamage_(std::pair<uint32_t, uint32_t> position) {
    DealDamage_(position, 2);
}

void GameField::Scan_(std::pair<uint32_t, uint32_t> position) {
    for (uint32_t i = 0; i < 2; ++i) {
        for (uint32_t j = 0; j < 2; ++j) {
            std::pair<uint32_t, uint32_t> offset = {i, j};
            std::pair<uint32_t, uint32_t> ability_position = {
                    position.first + offset.first, position.second + offset.second};
            CellStates state = shipmanager_.DealDamage(ability_position, 0);
            if (state == CellStates::DAMAGED || state == CellStates::ACTIVE) {
                std::cout << "There is an active ship in this area!" << std::endl;
                return;
            }
        }
    }
    std::cout << "There are no ships in this area!" << std::endl;
}

void GameField::Bombing_() {
    std::vector<std::pair<uint32_t, uint32_t>> cells;
    for (Ship& ship : shipmanager_.GetShips()) {
        if (ship.IsDestroyed()) {
            continue;
        }

        std::vector<CellStates> cell_states = ship.GetCellStates();
        for (size_t i = 0; i < ship.GetSize(); ++i) {
            std::pair<uint32_t, uint32_t> offset;
            if (ship.GetOrientation() == ShipOrientation::HORIZONTAL) {
                offset = {i, 0};
            } else {
                offset = {0, i};
            }

            if (cell_states[i] != CellStates::DESTROYED) {
                cells.push_back({ship.GetPosition().first + offset.first,
                                 ship.GetPosition().second + offset.second});
            }
        }
    }

    if (!cells.empty()) {
        std::pair<uint32_t, uint32_t> attack_position =
                cells[std::rand() % cells.size()];
        DealDamage_(attack_position, 1, true);
    }
}

bool GameField::is_free_cell(int x, int y, std::vector<std::pair<int, int>>& cells)
{
    std::pair<int, int> coords = {x, y};
    for (size_t i = 0; i < cells.size(); i++)
    {
        if (coords == cells[i])
            return true;
    }

    return false;
}

void GameField::InitEnemyShips(size_t pl_shipc, std::vector<size_t> pl_sizes)
{
    size_t count = pl_shipc;
    std::vector<std::pair<int, int>> is_free;

    for (int i = 0; i < (int)size_.first; i++)
    {
        for (int j = 0; j < (int)size_.second; j++)
        {
            is_free.push_back({i, j});
        }
    }

    for (size_t i = 0; i < count; i++)
    {
        int x = -1, y = -1;
        ShipOrientation orient;

        int attempts = 1000;
        while (attempts-- > 0)
        {
            orient = (i % 2 == 0 ? ShipOrientation::HORIZONTAL : ShipOrientation::VERTICAL);
            int correct_coord = (int)size_.first - (int)pl_sizes[i];

            if (correct_coord <= 0)
                correct_coord = 1;

            if (orient == ShipOrientation::HORIZONTAL)
            {
                x = rand() % correct_coord;
                y = rand() % size_.first;
            }
            else
            {
                x = rand() % size_.first;
                y = rand() % correct_coord;
            }

            if (is_free_cell(x, y, is_free))
                break;
        }

        if (attempts <= 0)
        {
            std::cerr << "Error: Unable to place ship after 100 attempts.\n";
            return;
        }

        auto ship = Ship(pl_sizes[i], {x, y}, orient);
        AddShip(ship);
    }
}

int GameField::GetWidth() const {
    return (int)size_.first;
}
