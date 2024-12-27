

function(CREATE_PALLADIUM_TEST TEST_NAME)
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp $<TARGET_OBJECTS:OBJECT_LIB>)
  target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/purge ${CMAKE_SOURCE_DIR}/include)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()


CREATE_PALLADIUM_TEST(LexerTest)
CREATE_PALLADIUM_TEST(TokenKindTest)
CREATE_PALLADIUM_TEST(VMMemoryTest)
CREATE_PALLADIUM_TEST(ParserTest)
