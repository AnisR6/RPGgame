cmake_minimum_required(VERSION 3.20)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project(RPGgame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX "/Main\\.cpp$")

add_executable(RPGgame src/Main.cpp)

add_library(game_logic STATIC
        ${SOURCES}
)
target_include_directories(game_logic PUBLIC
        "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(game_logic PUBLIC
        SFML::Graphics
        SFML::Window
        SFML::Audio
        SFML::System
        ImGui-SFML::ImGui-SFML
)

target_link_libraries(RPGgame PRIVATE
        game_logic
)


add_custom_command(
        TARGET RPGgame
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:RPGgame>/assets"
        COMMENT "Copy assets next to RPGgame executable")

enable_testing()
add_subdirectory(tests)
