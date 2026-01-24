cmake_minimum_required(VERSION 3.28.3)
project(pacman)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wpedantic)
#add_compile_options(-O0)

add_subdirectory(logic)

set(PACMAN_SOURCES
        main.cpp
        representation/AssetManager.cpp
        representation/Camera.cpp
        representation/Game.cpp
        representation/StateManager.cpp
        representation/ViewCompatibleEntityFactory.cpp
        representation/state/GameOverState.cpp
        representation/state/LevelState.cpp
        representation/state/MenuState.cpp
        representation/state/PausedState.cpp
        representation/state/VictoryState.cpp
        representation/view/CoinView.cpp
        representation/view/FruitView.cpp
        representation/view/GhostView.cpp
        representation/view/IDirectionalEntityView.cpp
        representation/view/IDirectionalEntityView.h
        representation/view/IEntityView.cpp
        representation/view/PacmanView.cpp
        representation/view/ScoreView.cpp
        representation/view/SpawnWallView.cpp
        representation/view/WallView.cpp
)

add_executable(pacman ${PACMAN_SOURCES})

target_link_libraries(pacman
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        GameLogic
)
