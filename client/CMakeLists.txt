cmake_minimum_required(VERSION 3.16)
project(SeaBattleClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

qt6_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/SeaBattleClient.cpp
    src/GameBoard.cpp
    src/GameLogic.cpp
    src/GameWidget.cpp
    src/demo_main.cpp
)

set(HEADERS
    src/SeaBattleClient.h
    src/GameBoard.h
    src/GameLogic.h
    src/GameWidget.h
    ../common/GameTypes.h
)

qt6_add_executable(SeaBattleClient ${SOURCES} ${HEADERS})

target_link_libraries(SeaBattleClient PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)

# Демо-приложение для отображения поля
set(DEMO_SOURCES
    src/demo_main.cpp
    src/GameBoard.cpp
    src/GameLogic.cpp
    src/GameWidget.cpp
)

set(DEMO_HEADERS
    src/GameBoard.h
    src/GameLogic.h
    src/GameWidget.h
    ../common/GameTypes.h
)

qt6_add_executable(SeaBattleDemo ${DEMO_SOURCES} ${DEMO_HEADERS})

target_link_libraries(SeaBattleDemo PRIVATE Qt6::Core Qt6::Widgets)

# Создаем директорию для логов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

