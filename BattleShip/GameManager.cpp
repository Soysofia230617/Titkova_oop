#include "GameManager.h"
#include <iostream>
#include <limits>
#include <queue>
#include <functional>
#include <sstream>
#include "GameState.h"

GameManager::GameManager() : ability_mgr_(enemy_field_), prev_destroyed_(0), is_player_turn_(true) {
}

void GameManager::Init() {
    InitFields_();
    InitShips_();

}

void GameManager::InitFields_()
{
    size_t size;

    std::cout << "Input size of field: ";
    std::cin >> size;

    while (std::cin.fail() || size == 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "INVALID SIZE! Please enter valid size of field: ";
        std::cin >> size;
    }

    player_field_ = GameField({size, size});
    enemy_field_ = GameField({size, size});
}

bool GameManager::GetCoordinates_(int& x, int& y) {
    int max_=player_field_.GetWidth();
    while (true) {
        std::cout << "Enter coordinates to attack the enemy: ";
        std::cin >> x >> y;
        if (std::cin.fail() || x < 1 || x > max_ || y < 1 || y > max_) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "INVALID COORDINATES! Please enter values between (1- " << max_<<
                      ") for both x and y." << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            x--;
            y--;
            return true;
        }
    }
}

void GameManager::EnemyTurn() {
    std::vector<std::pair<uint32_t, uint32_t>> vec;
    std::vector<std::vector<FieldCellStates>> field = player_field_.GetField();

    for (uint32_t i = 0; i < field.size(); ++i) {
        for (uint32_t j = 0; j < field[i].size(); ++j) {
            if (field[i][j] != FieldCellStates::DESTROYD &&
                field[i][j] != FieldCellStates::WATER) {
                vec.emplace_back(i, j);
            }
        }
    }

    if (!vec.empty()) {
        size_t rd = rand() % vec.size();
        uint32_t x = static_cast<uint32_t>(vec[rd].first);
        uint32_t y = static_cast<uint32_t>(vec[rd].second);

        std::cout << "Enemy attacks at (" << x + 1 << ", " << y + 1 << ")!"
                  << std::endl;

        player_field_.DealDamage({x, y});
    }

    is_player_turn_ = true;
}

bool GameManager::CheckGameOver() {
    if (enemy_field_.GetNumShips() == 0 && player_field_.GetNumShips() == 0) {
        return false;
    }

    if (enemy_field_.GetNumDestroyed() == enemy_field_.GetNumShips() && enemy_field_.GetNumShips() != 0) {
        std::cout << "You win!" << std::endl;
        return true;
    } else if (player_field_.GetNumDestroyed() == player_field_.GetNumShips() && player_field_.GetNumShips() != 0) {
        std::cout << "You lose! Starting a new game..." << std::endl;
        RestartGame();
        return true;
    }
    return false;
}


void GameManager::DisplayFields_() const{
    std::cout << "Your Field:" << std::endl;
    DisplayField_(player_field_);

    std::cout << "Enemy Field:" << std::endl;
    DisplayField_(enemy_field_);

    std::cout << std::endl;

    size_t player_destroyed = player_field_.GetNumDestroyed();
    size_t enemy_destroyed = enemy_field_.GetNumDestroyed();

    std::cout << "Destroyed Ships:" << std::endl
              << "You: " << player_destroyed << std::endl
              << "Enemy : " << enemy_destroyed << std::endl;
}

void GameManager::DisplayField_(const GameField& field) const {
    std::vector<std::vector<FieldCellStates>> grid = field.GetField();

    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == FieldCellStates::WATER) {
                std::cout << ". ";
            } else if (grid[i][j] == FieldCellStates::DESTROYD) {
                std::cout << "X ";
            } else if (grid[i][j] == FieldCellStates::UNKNOWN) {
                std::cout << "* ";
            } else {
                std::cout << "S ";
            }
        }
        std::cout << std::endl;
    }
}

void GameManager::DisplayAbilities_() const {
    std::queue<std::shared_ptr<Interface>> print_queue =
            ability_mgr_.GetAbilities();

    std::cout << "AVAILABLE ABILITIES: ";

    while (!print_queue.empty()) {
        if (print_queue.front()->GetType() == Abilities::DOUBLEDAMAGE) {
            std::cout << "DAMAGE ";
        } else if (print_queue.front()->GetType() == Abilities::SCAN) {
            std::cout << "SCAN ";
        } else if (print_queue.front()->GetType() == Abilities::BOMBING) {
            std::cout << "BOMBING ";
        }
        print_queue.pop();
    }

    std::cout << std::endl;
}

void GameManager::InitShips_() {

    size_t ship_count;
    std::cout << "Enter numbers of ships: ";
    std::cin >> ship_count;

    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Incorrect input. Enter numbers of ships: ";
        std::cin >> ship_count;
    }

    std::vector<size_t> ship_lengths(ship_count);
    for (int i = 0; i < (int)ship_count; ++i) {
        std::cout << "Enter the length of the ship " << (i + 1) << " (from 1 to 4): ";
        std::cin >> ship_lengths[i];

        while (std::cin.fail() || ship_lengths[i] < 1 || ship_lengths[i] > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "The length of the ship must be from 1 to 4. Repeat the entry: ";
            std::cin >> ship_lengths[i];
        }
    }

    player_field_.SetShipMgr(ship_count, ship_lengths);
    enemy_field_.SetShipMgr(ship_count, ship_lengths);

    for (int i = 0; i < (int)ship_count; ++i) {
        int x, y;
        char orientation;
        std::cout << "Enter coordinates (x y) and orientation (H or V) for a ship of length "
                  << ship_lengths[i] << ": ";
        std::cin >> x >> y >> orientation;

        while (std::cin.fail() || x < 1 || y < 1 ||
               (orientation != 'H' && orientation != 'h' && orientation != 'V' && orientation != 'v')) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter coordinates (x y) and orientation (H/V): ";
            std::cin >> x >> y >> orientation;
        }

        ShipOrientation orient;
        if (orientation == 'H' || orientation == 'h') {
            orient = ShipOrientation::HORIZONTAL;
        } else {
            orient = ShipOrientation::VERTICAL;
        }

        try {
            player_field_.AddShip(Ship(ship_lengths[i], {static_cast<uint32_t>(x - 1), static_cast<uint32_t>(y - 1)}, orient));
        } catch (const std::exception& e) {
            std::cerr << "Error adding ship: " << e.what() << std::endl;
            i--;
            continue;
        }
    }

    enemy_field_.InitEnemyShips(player_field_.GetNumShips(), ship_lengths);
}

void GameManager::SaveToStream_(std::ostream& out) const {
    std::queue<std::shared_ptr<Interface>> abilities_temp =
            ability_mgr_.GetAbilities();
    std::queue<Abilities> abilities;
    while (!abilities_temp.empty()) {
        abilities.push(abilities_temp.front()->GetType());
        abilities_temp.pop();
    }

    GameState game_state{player_field_, enemy_field_, abilities, prev_destroyed_};
    game_state.SaveToStream(out);
}

void GameManager::LoadFromStream_(std::istream& in) {
    std::queue<Abilities> abilities;
    GameState game_state{player_field_, enemy_field_, abilities, prev_destroyed_};
    game_state.LoadFromStream(in);
    player_field_ = game_state.GetPlayerField();
    enemy_field_ = game_state.GetEnemyField();
    abilities = game_state.GetAbilities();
    prev_destroyed_ = game_state.GetPrevDestroyed();
    ability_mgr_.Reset(enemy_field_);

    while (!abilities.empty()) {
        ability_mgr_.AddAbility(abilities.front());
        abilities.pop();
    }
}
void GameManager::SaveToFile(std::string& filename) const {
    std::ofstream out(filename, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        std::cout << "Error while opening a file!" << std::endl;
        return;
    }
    SaveToStream_(out);
    out.close();

    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "Error while opening the file for reading!" << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string fileContent = buffer.str();
    in.close();

    std::string hashValue = HashSum::CalculateHash(fileContent);

    std::ofstream hashOut("HashSum", std::ios::out | std::ios::app);
    if (hashOut.is_open()) {
        hashOut << hashValue << std::endl;
        hashOut.close();
    } else {
        std::cout << "Error while opening HashSum file!" << std::endl;
    }
}

void GameManager::LoadFromFile(std::string& filename) {
    std::ifstream in(filename, std::ios::in);
    if (!in.is_open()) {
        std::cout << "No such save!" << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string fileContent = buffer.str();
    in.close();

    std::ifstream hashIn("HashSum");
    if (!hashIn.is_open()) {
        std::cout << "Error while opening HashSum file!" << std::endl;
        return;
    }

    bool hashMatch = false;
    std::string storedHashValue;

    std::string currentHashValue = HashSum::CalculateHash(fileContent);

    while (std::getline(hashIn, storedHashValue)) {
        if (currentHashValue == storedHashValue) {
            hashMatch = true;
            break;
        }
    }

    hashIn.close();

    if (hashMatch) {
        std::istringstream tempStream(fileContent);
        LoadFromStream_(tempStream);
        std::cout << "File loaded successfully!" << std::endl;
    } else {
        std::cerr << "Error: Hash mismatch! The file may be corrupted." << std::endl;
    }
}


void GameManager::RestartGame() {
    prev_destroyed_ = 0;
    is_player_turn_ = true;
}

void GameManager::StartNextRound() {
    prev_destroyed_ = enemy_field_.GetNumDestroyed();
    is_player_turn_ = true;

    player_field_.Recover();
    enemy_field_.Recover();
    InitShips_();
}

void GameManager::DealDamageToEnemy(int x, int y) {
    enemy_field_.DealDamage({static_cast<uint32_t>(x), static_cast<uint32_t>(y)});
}

void GameManager::UseAbility() {
    try {
        ability_mgr_.UseAbility();
    } catch (NoAbility&) {
        std::cout << "No abilities available!" << std::endl;
    }
}
