
add_executable(TokenKindTest TokenKindTest.cpp $<TARGET_OBJECTS:OBJECT_LIB>)
target_include_directories(TokenKindTest PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/purge ${CMAKE_SOURCE_DIR}/include)
