cmake_minimum_required(VERSION 3.16)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Добавляем подпроекты
add_subdirectory(server)
add_subdirectory(client)
