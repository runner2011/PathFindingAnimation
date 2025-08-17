cmake_minimum_required(VERSION 3.10)
project(PathFindingAnim)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE ENGINE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/engine/*.cpp")
file(GLOB_RECURSE GAME_SOURCE_FILES "${CMAKE_SOURCE_DIR}/game/*.cpp")

add_executable(PathFindingAnim 
${ENGINE_SOURCE_FILES}
${GAME_SOURCE_FILES}
)


target_include_directories(PathFindingAnim PUBLIC ${CMAKE_SOURCE_DIR}/engine)
target_include_directories(PathFindingAnim PUBLIC ${CMAKE_SOURCE_DIR}/game/src)

set(CMAKE_BUILD_TYPE Debug)