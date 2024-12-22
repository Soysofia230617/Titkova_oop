#include "ShipManager.h"

void ShipManager::AddShip(Ship new_ship) {
    if (shipc_ == 0) {
        throw NoShipsLeft();
    }

    if (!ShipManager::HasOverlap(new_ship)) {
        for (size_t i = 0; i < sizes_.size(); ++i) {
            if (sizes_[i] == new_ship.GetSize()) {
                sizes_.erase(sizes_.begin() + i);
                ships_.push_back(new_ship);
                --shipc_;
                return;
            }
        }

        throw NoSuchShipSize();
    }

    throw Overlap();
}

bool ShipManager::HasOverlap(Ship ship) {
    for (Ship& mgr_ship : ships_) {
        for (size_t i = 0; i < mgr_ship.GetSize(); ++i) {
            for (size_t j = 0; j < ship.GetSize(); ++j) {
                std::pair<uint32_t, uint32_t> offset1;
                std::pair<uint32_t, uint32_t> offset2;

                if (mgr_ship.GetOrientation() == ShipOrientation::HORIZONTAL) {
                    offset1 = {i, 0};
                } else {
                    offset1 = {0, i};
                }
                if (ship.GetOrientation() == ShipOrientation::HORIZONTAL) {
                    offset2 = {i, 0};
                } else {
                    offset2 = {0, i};
                }

                std::pair<uint32_t, uint32_t> point1 = {
                        mgr_ship.GetPosition().first + offset1.first,
                        mgr_ship.GetPosition().second + offset1.second};
                std::pair<uint32_t, uint32_t> point2 = {
                        ship.GetPosition().first + offset2.first,
                        ship.GetPosition().second + offset2.second};

                if (point1 == point2) {
                    return true;
                }

                // Проверка соседних клеток
                std::pair<uint32_t, uint32_t> neighbours[8] = {
                        {point1.first + 1, point1.second},
                        {point1.first + 1, point1.second + 1},
                        {point1.first,     point1.second + 1},
                        {point1.first - 1, point1.second + 1},
                        {point1.first - 1, point1.second},
                        {point1.first - 1, point1.second - 1},
                        {point1.first,     point1.second - 1},
                        {point1.first + 1, point1.second - 1}
                };

                for (auto &n: neighbours) {
                    if (point2 == n) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

CellStates ShipManager::DealDamage(std::pair<uint32_t, uint32_t> position,uint32_t damage) {
    for (Ship& ship : ships_) {
        CellStates state = ship.DealDamage(position, damage);
        if (state != CellStates::WATER) {
            return state;
        }
    }

    return CellStates::WATER;
}

std::vector<Ship> ShipManager::GetShips() const{
    return ships_;
}
size_t ShipManager::GetNumShips() const {
    return ships_.size();
}

void ShipManager::SaveToStream(std::ostream& out) const {
    out << shipc_ << std::endl;
    for (size_t i = 0; i < shipc_; ++i) {
        out << sizes_[i] << " ";
    }
    out << std::endl;
    out << ships_.size() << std::endl;
    for (size_t i = 0; i < ships_.size(); ++i) {
        ships_[i].SaveToStream(out);
    }
}

void ShipManager::LoadFromStream(std::istream& in) {
    in >> shipc_;
    sizes_.clear();
    sizes_.resize(shipc_);
    for (size_t i = 0; i < shipc_; ++i) {
        in >> sizes_[i];
    }
    ships_.clear();
    size_t sz;
    in >> sz;
    for (size_t i = 0; i < sz; ++i) {
        Ship ship(0, {0, 0}, ShipOrientation::HORIZONTAL);
        ship.LoadFromStream(in);
        ships_.push_back(ship);
    }
}

const std::vector<size_t> ShipManager::GetSizes() {
    return sizes_;
}
