# Pacman Gevorderd Programmeren

**CouldBeMathijs - 2024xxxx**

## Core Game Requirements

- [x] Startup screen + scoreboard _See MenuState.h_
- [x] Maze & collision correctness
- [x] Smooth continuous movement
- [x] Ghost AI (4 modes, Manhattan distance, direction locking) _See IGhost.h and World.cpp_
- [x] Coin/fruit score modifiers _See ScoreKeeper.cpp_
- [x] Fear mode + ghost reversal _See IGhost.h and World.cpp_
- [x] Level clearing + scaling difficulty _See ScoreKeeper.cpp, LevelState.cpp and World.cpp_
- [x] Life system & game over _See ScoreKeeper.cpp, LevelState.cpp and World.cpp_

## Soft. Design & Code Arch.
- [x] Clear MVC separation _Models do not know views exist, they only see Observers_
- [x] Patterns: 
  - [x] MVC _See `ViewCompatibleEntityFactory.h`, `IEntity.h`_
  - [x] Observer _See `IObserver.h`, `ISubject.h`_
  - [x] Abstract Factory _See `AbstractFactory.h`_
  - [x] Singleton _See `Camera.h`, `Random.h`_
  - [x] State _See the `state` directory_
- [x] Logic as standalone library _See GitHub Actions, `CMakeList.txt`
- [x] Camera & Normalized coords _See `WorldCreator.cpp`_
- [x] Good polymorphism & extensibility _Ex. ISubject > IEntityModel > IDirectionalEntityModel > IGhost > RedGhost_

## Bonus Features

- [x] Alternative build system _See `meson.build`_
- [x] Extra design patterns _See Visitor.h_
- [ ] Extra gameplay/visuals
- [ ] Multithreading
- [ ] Procedural maps
- [ ] Smarter AI (BFS, A*)
- [ ] Sounds/music
