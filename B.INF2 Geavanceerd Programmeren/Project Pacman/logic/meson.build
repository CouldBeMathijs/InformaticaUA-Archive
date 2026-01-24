# Define include directory for this library
logic_inc = include_directories('.')

logic_sources = files(
    'entityType/IDirectionalEntityModel.cpp',
    'entityType/IEntityModel.cpp',
    'entityType/ISubject.cpp',
    'Position.cpp',
    'Random.cpp',
    'ScoreKeeper.cpp',
    'Stopwatch.cpp',
    'patterns/Visitor.cpp',
    'World.cpp',
    'mapLoading/WorldAsciiGridParser.cpp',
    'mapLoading/WorldCreator.cpp',
    'entityType/Collectable.cpp',
    'entityType/Ghost.cpp',
    'entityType/Pacman.cpp',
    'entityType/Wall.cpp',
    'entityType/SpawnWall.cpp'
)

# Build the static library
game_logic_lib = static_library('GameLogic',
                                logic_sources,
                                include_directories : logic_inc
)
