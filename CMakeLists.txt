cmake_minimum_required(VERSION 3.13)
project(Texas_Hold_em)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lboost_thread")
set(Boost_INCLUDE_DIR /usr/local/lib)
find_package(Boost REQUIRED thread chrono)

add_executable(Texas_Hold_em
  main.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        Deck/FaceValues.hpp
        NetworkAssests/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        Player/Player.cpp
        Player/Player.hpp
        NetworkAssests/NetworkController.cpp
        NetworkAssests/NetworkController.hpp
        NetworkAssests/Package.hpp
        NetworkAssests/Package.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  Boost::thread
  Boost::chrono)

add_executable(Tester
  Tester.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        Deck/FaceValues.hpp
        NetworkAssests/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        Player/Player.cpp
        Player/Player.hpp
        NetworkAssests/NetworkController.cpp
        NetworkAssests/NetworkController.hpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        NetworkAssests/Package.hpp
        NetworkAssests/Package.cpp)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(
  Tester
  Boost::thread
  Boost::chrono)

add_executable(TexasClient
        Deck/FaceValues.hpp
        NetworkAssests/Client.cpp
        NetworkAssests/Update.hpp
        NetworkAssests/Package.cpp
        NetworkAssests/Package.hpp)

target_link_libraries(
  TexasClient
  Boost::thread
  Boost::chrono)

add_executable(TexasServer
        NetworkAssests/TexasServer.hpp
  main.cpp
        Deck/Deck.hpp
        Deck/Deck.cpp
        GameAssets/RandomEngine.cpp
        GameAssets/RandomEngine.hpp
        Deck/FaceValues.hpp
        NetworkAssests/Table.cpp
        Deck/Game.cpp
        Deck/Game.hpp
        NetworkAssests/Package.cpp
        Player/Player.cpp
        NetworkAssests/NetworkController.cpp)

target_link_libraries(
  TexasServer
  Boost::thread
  Boost::chrono)
