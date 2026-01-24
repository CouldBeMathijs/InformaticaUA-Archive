cmake_minimum_required(VERSION 3.30)
project(W13Woe)

set(CMAKE_CXX_STANDARD 11)

add_executable(W13Woe main.cpp
        World.cpp
        World.h
        Level.cpp
        Level.h
        Room.cpp
        Room.h
        TerrainRoom.cpp
        TerrainRoom.h
        DungeonRoom.cpp
        DungeonRoom.h
        Item.cpp
        Item.h
        PotionItem.cpp
        PotionItem.h
        WeaponItem.cpp
        WeaponItem.h
        Character.cpp
        Character.h
        MonsterCharacter.cpp
        MonsterCharacter.h
        HeroCharacter.cpp
        HeroCharacter.h)
