#include "Ship.h"

std::vector<CellStates> Ship::GetCellStates() const {
    return states_;
}

std::pair<uint32_t, uint32_t> Ship::GetPosition() const {
    return position_;
}

std::ostream& Ship::SaveToStream(std::ostream& out) const {
    out << size_ << std::endl;
    out << position_.first << " " << position_.second << std::endl;
    out << static_cast<int32_t>(orientation_) << std::endl;
    for (size_t i = 0; i < states_.size(); ++i) {
        out << static_cast<int32_t>(states_[i]) << " ";
    }
    out << std::endl;
    return out;
}

std::istream& Ship::LoadFromStream(std::istream& in) {
    int32_t orientation;
    in >> size_;
    in >> position_.first >> position_.second;
    in >> orientation;
    orientation_ = static_cast<ShipOrientation>(orientation);
    states_.resize(size_);
    for (size_t i = 0; i < size_; ++i) {
        int32_t state;
        in >> state;
        states_[i] = static_cast<CellStates>(state);
    }
    return in;
}

size_t Ship::GetSize() const {
    return states_.size();
}

bool Ship::IsDestroyed() const {
    return std::count(states_.begin(), states_.end(), CellStates::DESTROYED) == states_.size();
}

CellStates Ship::DealDamage(std::pair<uint32_t, uint32_t> position, uint32_t damage) {
    for (size_t i = 0; i < size_; ++i) {
        std::pair<uint32_t, uint32_t> offset;

        if (orientation_ == ShipOrientation::HORIZONTAL) {
            offset = {i, 0};
        } else {
            offset = {0, i};
        }

        if (position_.first + offset.first == position.first &&
            position_.second + offset.second == position.second) {
            if (damage < static_cast<uint32_t>(states_[i])) {
                states_[i] = static_cast<CellStates>(
                        static_cast<uint32_t>(states_[i]) - damage);
                return states_[i];
            } else {
                states_[i] = CellStates::DESTROYED;
                return states_[i];
            }
        }
    }

    return CellStates::WATER;
}

ShipOrientation Ship::GetOrientation() const {
    return orientation_;
}
