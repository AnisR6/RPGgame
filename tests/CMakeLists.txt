find_package(Catch2 CONFIG REQUIRED)

add_executable(tests
        unit/BattleSystemTest.cpp
)

target_link_libraries(tests PRIVATE
        game_logic
        Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(tests
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

