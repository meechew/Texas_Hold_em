cmake_minimum_required(VERSION 3.13)
project(Texas_Hold_em)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_FAKER_TESTS OFF)
add_subdirectory(external/faker-cxx)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_THREAD_VERSION=4")
find_package(Boost CONFIG REQUIRED atomic chrono thread)

add_executable(
        Texas_Hold_em
        src/main.cpp
        src/Deck/Deck.hpp
        src/Deck/Deck.cpp
        src/GameAssets/RandomEngine.cpp
        src/GameAssets/RandomEngine.hpp
        src/Deck/FaceValues.hpp
        src/NetworkAssets/Table.cpp
        src/Deck/Game.cpp
        src/Deck/Game.hpp
        src/Player/Player.cpp
        src/Player/Player.hpp
        src/NetworkAssets/NetworkController.cpp
        src/NetworkAssets/NetworkController.hpp
        src/NetworkAssets/Package.hpp
        src/NetworkAssets/Package.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        Boost::thread
        Boost::chrono)

add_executable(
        Tester
        src/Tester.cpp
        src/Deck/Deck.hpp
        src/Deck/Deck.cpp
        src/Deck/FaceValues.hpp
        src/NetworkAssets/Table.cpp
        src/Deck/Game.cpp
        src/Deck/Game.hpp
        src/Player/Player.cpp
        src/Player/Player.hpp
        src/NetworkAssets/NetworkController.cpp
        src/NetworkAssets/NetworkController.hpp
        src/GameAssets/RandomEngine.cpp
        src/GameAssets/RandomEngine.hpp
        src/NetworkAssets/Package.hpp
        src/NetworkAssets/Package.cpp)

include_directories(${Boost_INCLUDE_DIR}
        external/faker-cxx)
target_link_libraries(
        Tester
        faker-ccov-all
        Boost::thread
        Boost::chrono)

add_executable(
        TexasClient
        src/Deck/FaceValues.hpp
        src/NetworkAssets/Client.cpp
        src/NetworkAssets/Update.hpp
        src/NetworkAssets/Package.cpp
        src/NetworkAssets/Package.hpp)

target_link_libraries(
        TexasClient
        Boost::thread
        Boost::chrono
        Boost::atomic
        pthread
        rt)

add_executable(
        TexasServer
        src/NetworkAssets/TexasServer.hpp
        src/main.cpp
        src/Deck/Deck.hpp
        src/Deck/Deck.cpp
        src/GameAssets/RandomEngine.cpp
        src/GameAssets/RandomEngine.hpp
        src/Deck/FaceValues.hpp
        src/NetworkAssets/Table.cpp
        src/Deck/Game.cpp
        src/Deck/Game.hpp
        src/NetworkAssets/Package.cpp
        src/Player/Player.cpp
        src/NetworkAssets/NetworkController.cpp)

target_link_libraries(
        TexasServer
        Boost::thread
        Boost::chrono)
