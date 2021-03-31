set(LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libpugcc")

include_directories(${LIB_SOURCE_DIR}/include)

add_gtest(lexer.source lexer/source.test.cpp)

add_gtest(lexer.lexer
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

add_gtest(io.output_buffer
  io/output_buffer.test.cpp
  ${LIB_SOURCE_DIR}/include/pugcc/io/output_buffer.h
)

add_gtest(ast.element
  ast/element.test.cpp
  ${LIB_SOURCE_DIR}/ast/element.h
  ${LIB_SOURCE_DIR}/ast/element.cpp
)
