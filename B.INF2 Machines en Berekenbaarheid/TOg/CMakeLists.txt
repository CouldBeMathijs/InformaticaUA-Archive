cmake_minimum_required(VERSION 3.20)
project(MBTOg)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#add_compile_options(-Wall)

set(MBTOG_SOURCES
        CFG.cpp
        PDA.cpp
        SLRParser.cpp
        ShuntingYard.cpp
        ast/AST.cpp
        ast/ExpressionOperatorConfig.cpp
        ast/OperatorEnvironment.cpp
        config/OperatorConfig.cpp
        external/json.hpp
        gui/ExpressionApp.cpp
        main.cpp
        utils/Tokenizer.cpp
)

add_executable(MBTOg ${MBTOG_SOURCES})
target_link_libraries(MBTOg PRIVATE sfml-graphics sfml-window sfml-system)

# --------------------------------------
# TESTS
# --------------------------------------

enable_testing()

# AST Test (Repeat this block for other tests)
add_executable(test_ast
        tests/test_ast.cpp
        ast/AST.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp)
add_test(NAME ASTTests COMMAND test_ast)

# Parser Test
add_executable(test_parser tests/test_parser.cpp
        CFG.cpp
        SLRParser.cpp
        ast/AST.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp
        config/OperatorConfig.cpp
)
target_include_directories(test_parser PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
)
add_test(NAME ParserTests COMMAND test_parser)

add_executable(test_parser2 tests/test_parser2.cpp
        CFG.cpp
        SLRParser.cpp
        ast/AST.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp
        config/OperatorConfig.cpp
)
target_include_directories(test_parser2 PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
)

add_test(NAME ParserTest2 COMMAND test_parser2)


# SLR Internal Algorithm Tests
add_executable(test_slr_internals tests/test_slr_internals.cpp
        CFG.cpp
        SLRParser.cpp
        PDA.cpp
        ast/AST.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp
        config/OperatorConfig.cpp

)
target_include_directories(test_slr_internals PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
)
add_test(NAME SLRInternalTests COMMAND test_slr_internals)

# AST Edge Case Tests
add_executable(test_ast_edge_cases tests/test_ast_edge_cases.cpp
        ast/AST.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp
)
target_include_directories(test_ast_edge_cases PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
)
add_test(NAME ASTEdgeCaseTests COMMAND test_ast_edge_cases)


# PDA Simulation Tests
add_executable(test_pda tests/test_pda.cpp
        PDA.cpp
        CFG.cpp
        config/OperatorConfig.cpp
)
target_include_directories(test_pda PRIVATE
        ${CMAKE_SOURCE_DIR}
)
add_test(
        NAME PDATests
        COMMAND test_pda
)

# Shunting Yard Tests
add_executable(test_shunting_yard
        tests/test_shunting_yard.cpp
        ShuntingYard.cpp
        utils/Tokenizer.cpp
        ast/OperatorEnvironment.cpp
        ast/ExpressionOperatorConfig.cpp
        config/OperatorConfig.cpp
)
target_include_directories(test_shunting_yard PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/ast
        ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_SOURCE_DIR}/utils
)
add_test(NAME ShuntingYardTests COMMAND test_shunting_yard)

#Tokenizer tests
add_executable(test_tokenizer
        tests/test_tokenizer.cpp
        utils/Tokenizer.cpp
        config/OperatorConfig.cpp
)

target_include_directories(test_tokenizer PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/utils
        ${CMAKE_SOURCE_DIR}/config
)
add_test(NAME TokenizerTests COMMAND test_tokenizer)