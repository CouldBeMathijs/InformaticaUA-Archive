project('pacman', 'cpp',
  version : '0.1',
  default_options : [
    # 'warning_level=3',
    'cpp_std=c++23',
    'werror=false'
    ]
  )


# We try to find SFML on the system first. 
# If not found, it triggers the 'sfml' subproject defined in subprojects/sfml.wrap
sfml_graphics_dep = dependency('sfml-graphics', version: '>=2.5', fallback: ['sfml', 'sfml_graphics_dep'])
sfml_window_dep   = dependency('sfml-window',   version: '>=2.5', fallback: ['sfml', 'sfml_window_dep'])
sfml_system_dep   = dependency('sfml-system',   version: '>=2.5', fallback: ['sfml', 'sfml_system_dep'])

sfml_deps = [sfml_graphics_dep, sfml_window_dep, sfml_system_dep]
# Enter the logic subdirectory
subdir('logic')

# Main executable sources
pacman_sources = files(
  'main.cpp',
  'representation/AssetManager.cpp',
  'representation/Camera.cpp',
  'representation/Game.cpp',
  'representation/StateManager.cpp',
  'representation/ViewCompatibleEntityFactory.cpp',
  'representation/state/GameOverState.cpp',
  'representation/state/LevelState.cpp',
  'representation/state/MenuState.cpp',
  'representation/state/PausedState.cpp',
  'representation/state/VictoryState.cpp',
  'representation/view/CoinView.cpp',
  'representation/view/FruitView.cpp',
  'representation/view/GhostView.cpp',
  'representation/view/IDirectionalEntityView.cpp',
  'representation/view/IEntityView.cpp',
  'representation/view/PacmanView.cpp',
  'representation/view/ScoreView.cpp',
  'representation/view/SpawnWallView.cpp',
  'representation/view/WallView.cpp'
  )

# Create the executable
executable('pacman',
  pacman_sources,
  include_directories : logic_inc, # From the logic subdir
  link_with : game_logic_lib,     # From the logic subdir
  dependencies : sfml_deps,
  install : true
  )
