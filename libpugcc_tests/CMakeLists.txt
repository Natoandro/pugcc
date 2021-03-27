set(LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libpugcc")

add_gtest(lexer_source lexer/source.test.cpp)

add_gtest(lexer_lexer
  lexer/lexer.test.cpp
  ${LIB_SOURCE_DIR}/lexer/lexer.cpp
  ${LIB_SOURCE_DIR}/lexer/lexer.h
  ${LIB_SOURCE_DIR}/lexer/source.h
  ${LIB_SOURCE_DIR}/error.h
)

add_gtest(utils.record
  utils/record.test.cpp
  ${LIB_SOURCE_DIR}/utils/record.h
)
