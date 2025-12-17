cmake_minimum_required(VERSION 3.13)
project(Texas_Hold_em)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_THREAD_VERSION=4")
find_package(Boost CONFIG REQUIRED atomic chrono thread)

add_executable(
        Texas_Hold_em
        main.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        Deck/FaceValues.hpp
        NetworkAssets/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        Player/Player.cpp
        Player/Player.hpp
        NetworkAssets/NetworkController.cpp
        NetworkAssets/NetworkController.hpp
        NetworkAssets/Package.hpp
        NetworkAssets/Package.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        Boost::thread
        Boost::chrono)

add_executable(
        Tester
        Tester.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        Deck/FaceValues.hpp
        NetworkAssets/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        Player/Player.cpp
        Player/Player.hpp
        NetworkAssets/NetworkController.cpp
        NetworkAssets/NetworkController.hpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        NetworkAssets/Package.hpp
        NetworkAssets/Package.cpp)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(
        Tester
        Boost::thread
        Boost::chrono)

add_executable(
        TexasClient
        Deck/FaceValues.hpp
        NetworkAssets/Client.cpp
        NetworkAssets/Update.hpp
        NetworkAssets/Package.cpp
        NetworkAssets/Package.hpp)

target_link_libraries(
        TexasClient
        Boost::thread
        Boost::chrono
        Boost::atomic
        pthread
        rt)

add_executable(
        TexasServer
        NetworkAssets/TexasServer.hpp
        main.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        Deck/FaceValues.hpp
        NetworkAssets/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        NetworkAssets/Package.cpp
        Player/Player.cpp
        NetworkAssets/NetworkController.cpp)

target_link_libraries(
        TexasServer
        Boost::thread
        Boost::chrono)
