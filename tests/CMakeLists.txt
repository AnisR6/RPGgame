find_package(Catch2 CONFIG REQUIRED)

add_executable(tests
        example_test.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME AllTests COMMAND tests)