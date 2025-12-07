cmake_minimum_required(VERSION 3.15)
project(Proiect_POO)

set(CMAKE_CXX_STANDARD 17)


set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES TRUE)


add_subdirectory(SFML)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/SFML/include")


add_executable(Proiect_POO main.cpp Player.cpp Player.h
        Enemy.cpp
        Enemy.h
        Platform.cpp
        Platform.h
        Enums.h
        Level.cpp
        Level.h
        Powerup.cpp
        Powerup.h
        Exceptions.h
)


target_link_libraries(Proiect_POO PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_definitions(Proiect_POO PRIVATE SFML_STATIC)