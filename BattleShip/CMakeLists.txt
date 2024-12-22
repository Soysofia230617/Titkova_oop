cmake_minimum_required(VERSION 3.26.4)
project(BattleShip)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp
        HashSum.h
        HashSum.cpp
        GameController.h
        GameDisplay.h
        ConsoleRender.h
        Commands.h
        CommandProvider.h
        CommandProvider.cpp
        ConsoleRender.cpp
)

add_library(interface Interface.h Interface.cpp)
add_library(gamefield GameField.h GameField.cpp)
add_library(ship Ship.h Ship.cpp)
add_library(shipmgr ShipManager.h ShipManager.cpp)
add_library(abilitymgr AbilityManager.h AbilityManager.cpp)
add_library(expectation Expectation.h Expectation.cpp)
add_library(gamemgr GameManager.h GameManager.cpp)
add_library(enums Enum.h Enum.cpp)
add_library(gamestate GameState.cpp GameState.h)

target_link_libraries(${PROJECT_NAME} gamemgr gamestate interface abilitymgr gamefield shipmgr ship enums expectation)
