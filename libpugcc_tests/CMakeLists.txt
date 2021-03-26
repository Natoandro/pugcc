
add_executable(lexer
  lexer/source.test.cpp
)

target_link_libraries(lexer gtest_main)

add_test(NAME lexer_test COMMAND lexer)
