#include "HashSum.h"

std::string HashSum::CalculateHash(const std::string& data) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(data);
    return std::to_string(hashValue);
}
