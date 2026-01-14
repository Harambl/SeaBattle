cmake_minimum_required(VERSION 3.16)
project(OceanServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)
qt6_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/OceanServer.cpp
)

set(HEADERS
    src/OceanServer.h
)

qt6_add_executable(OceanServer ${SOURCES} ${HEADERS})
target_link_libraries(OceanServer PRIVATE Qt6::Core Qt6::Network)

# Создаем директорию для логов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

