cmake_minimum_required(VERSION 3.16)
project(Texas_Hold_em)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lboost_thread")
set(Boost_INCLUDE_DIR /usr/local/lib)
find_package(Boost REQUIRED thread chrono)

add_executable(Texas_Hold_em
        main.cpp
        Deck.hpp
        Deck.cpp
        RandomEngine.cpp
        RandomEngine.hpp
        FaceValues.hpp
        Table.cpp
        Game.cpp
        Game.hpp
        Player.cpp
        Player.hpp
        NetworkController.cpp
        NetworkController.hpp
        Package.hpp
        Package.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        Boost::thread
        Boost::chrono)

add_executable(Tester
        Tester.cpp
        Deck.hpp
        Deck.cpp
        FaceValues.hpp
        Table.cpp
        Game.cpp
        Game.hpp
        Player.cpp
        Player.hpp
        NetworkController.cpp
        NetworkController.hpp
        RandomEngine.cpp
        RandomEngine.hpp
        Package.hpp
        Package.cpp
        Client.cpp)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(
        Tester
        Boost::thread
        Boost::chrono
        )

#add_executable(Client
        #Client.cpp
        #Update.hpp)

#add_executable(Server
        #Server.cpp
        #Update.hpp)