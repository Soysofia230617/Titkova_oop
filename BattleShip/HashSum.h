
#ifndef BATTLESHIP_HASHSUM_H
#define BATTLESHIP_HASHSUM_H
#include <string>
#include <functional>

class HashSum {
public:
    static std::string CalculateHash(const std::string& data);
};

#endif //BATTLESHIP_HASHSUM_H
