cmake_minimum_required(VERSION 3.13)
project(Texas_Hold_em)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_FAKER_TESTS OFF)
add_subdirectory(external/faker-cxx)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_THREAD_VERSION=4")
find_package(Boost CONFIG REQUIRED atomic chrono thread unit_test_framework)

add_executable(
        Texas_Hold_em
        src/main.cpp
        src/Deck/Deck.hpp
        src/Deck/Deck.cpp
        src/GameUtilities/RandomEngine.cpp
        src/GameUtilities/RandomEngine.hpp
        src/Deck/FaceValues.hpp
        src/NetworkAssets/Table.cpp
        src/Games/Game.hpp
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
        Tests/Tester.cpp
        src/Deck/Deck.hpp
        src/Deck/Deck.cpp
        src/Deck/FaceValues.hpp
        src/Games/Game.hpp
        src/Player/Player.cpp
        src/Player/Player.hpp
        src/NetworkAssets/NetworkController.cpp
        src/NetworkAssets/NetworkController.hpp
        src/GameUtilities/RandomEngine.cpp
        src/GameUtilities/RandomEngine.hpp
        src/NetworkAssets/Package.hpp
        src/NetworkAssets/Package.cpp
        src/GameTable/GameTable.cpp
        src/GameTable/GameTable.hpp
        src/Games/TexasHoldEm.cpp
        src/Games/TexasHoldEm.hpp)

include_directories(${Boost_INCLUDE_DIR}
        external/faker-cxx)
target_link_libraries(
        Tester
        faker-cxx
        ${Boost_LIBRARIES})

add_executable(
        TexasClient
        src/Deck/FaceValues.hpp
        src/Client/Client.cpp
        src/NetworkAssets/Message.hpp
        src/NetworkAssets/Package.cpp
        src/NetworkAssets/Package.hpp
        src/Client/StartClient.cpp)

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
        src/GameUtilities/RandomEngine.cpp
        src/GameUtilities/RandomEngine.hpp
        src/Deck/FaceValues.hpp
        src/Games/Game.hpp
        src/Games/TexasHoldEm.hpp
        src/Games/TexasHoldEm.cpp
        src/NetworkAssets/Package.cpp
        src/Player/Player.cpp
        src/NetworkAssets/NetworkController.cpp)

target_link_libraries(
        TexasServer
        Boost::thread
        Boost::chrono)
