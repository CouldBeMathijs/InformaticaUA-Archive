set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LOGIC_SOURCES
        entityType/IDirectionalEntityModel.cpp
        entityType/IEntityModel.cpp
        entityType/ISubject.cpp
        Position.cpp
        Random.cpp
        ScoreKeeper.cpp
        Stopwatch.cpp
        patterns/Visitor.cpp
        World.cpp
        mapLoading/WorldAsciiGridParser.cpp
        mapLoading/WorldCreator.cpp
        entityType/Collectable.cpp
        entityType/Ghost.cpp
        entityType/Pacman.cpp
        entityType/Wall.cpp
        entityType/SpawnWall.cpp
        entityType/SpawnWall.h
)
add_compile_options(-Wpedantic)

add_library(GameLogic STATIC ${LOGIC_SOURCES})

target_include_directories(GameLogic
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)