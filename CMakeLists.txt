cmake_minimum_required(VERSION 3.16)
project(WarehouseRobotSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES src/*.cpp)

add_executable(WarehouseRobotSim ${SOURCES}
        src/System.cpp
        include/System.h
        src/RobotManager.cpp
        include/RobotManager.h
        src/FileManager.cpp
        include/FileManager.h
        src/Position.cpp
        src/Warehouse.cpp
)