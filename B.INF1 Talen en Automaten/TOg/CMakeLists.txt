cmake_minimum_required(VERSION 3.30)
project(TA_TOg)

set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (default Release)" FORCE)
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(TA_TOg main.cpp
        Adriaan/RE.cpp
        Adriaan/ENFA.cpp
        Norkella/DFA.cpp
        Norkella/ENFA.cpp
        Norkella/RE.cpp
        Languages.cpp
        # GUI
        Gui/Application.cpp
        Gui/MainGui.cpp
        Gui/ui/Button.cpp
        Gui/ui/Label.cpp
        Gui/ui/TextInput.cpp
        Gui/ui/TextArea.cpp
        Gui/ui/Checkbox.cpp
        Gui/ui/Card.cpp
        Gui/ui/Tabs.cpp
        Norkella/General.cpp
        Norkella/General.h
        TokenMatcher.cpp
        TokenMatcher.h
)

add_executable(GenLang genlang.cpp
        Adriaan/RE.cpp
        Adriaan/ENFA.cpp
        Norkella/DFA.cpp
        Norkella/ENFA.cpp
        Norkella/RE.cpp
        Languages.cpp
        Norkella/General.cpp
        Norkella/General.h
        TokenMatcher.cpp
        TokenMatcher.h
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(TA_TOg
        sfml-graphics
        sfml-window
        sfml-system
)