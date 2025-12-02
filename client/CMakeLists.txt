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
)

set(HEADERS
    src/SeaBattleClient.h
)

qt6_add_executable(SeaBattleClient ${SOURCES} ${HEADERS})

target_link_libraries(SeaBattleClient PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)

# Создаем директорию для логов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

