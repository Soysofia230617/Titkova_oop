#ifndef BATTTLESIP_SHIP_H
#define BATTTLESIP_SHIP_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

#define SHIP_MIN_SIZE size_t(1)
#define SHIP_MAX_SIZE size_t(4)

enum class CellStates { DESTROYED = 0, DAMAGED = 1, ACTIVE = 2, WATER = 3 };
enum class ShipOrientation { HORIZONTAL = 0, VERTICAL = 1 };

class Ship {
public:
    Ship(size_t size, std::pair<uint32_t, uint32_t> position,
         ShipOrientation orientation)
            : states_(std::vector<CellStates>(size, CellStates::ACTIVE)),
              size_(std::min(std::max(size, SHIP_MIN_SIZE), SHIP_MAX_SIZE)),
              position_(position),
              orientation_(orientation) {}
    std::vector<CellStates> GetCellStates() const;
    std::pair<uint32_t, uint32_t> GetPosition() const;
    std::ostream& SaveToStream(std::ostream& out) const;
    std::istream& LoadFromStream(std::istream& in);
    bool IsDestroyed() const;
    size_t GetSize() const;
    ShipOrientation GetOrientation() const;
    CellStates DealDamage(std::pair<uint32_t, uint32_t> position, uint32_t damage);

private:
    std::vector<CellStates> states_;
    std::pair<uint32_t, uint32_t> position_;
    size_t size_;
    ShipOrientation orientation_;
};

#endif  // BATTTLESIP_SHIP_H
