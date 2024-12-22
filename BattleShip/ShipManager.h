#ifndef BATTTLESIP_SHIPMANAGER_H
#define BATTTLESIP_SHIPMANAGER_H

#include <iostream>

#include "Expectation.h"
#include "Ship.h"

class ShipManager {
public:
    ShipManager(size_t shipc, std::vector<size_t> sizes) : shipc_(shipc), sizes_(sizes) {}
    void AddShip(Ship new_ship);
    CellStates DealDamage(std::pair<uint32_t, uint32_t> position, uint32_t damage);
    std::vector<Ship> GetShips() const;
    size_t GetNumShips() const;
    const std::vector<size_t> GetSizes();
    void SetShipCount(int count);
    void SetShipLengths(std::vector<int>);
    void SaveToStream(std::ostream& out) const;
    void LoadFromStream(std::istream& in);

private:
    std::vector<Ship> ships_;
    std::vector<size_t> sizes_;
    size_t shipc_;
    bool HasOverlap(Ship ship);
};
#endif // BATTTLESIP_SHIPMANAGER_H