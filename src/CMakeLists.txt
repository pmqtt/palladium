
target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_library(OBJECT_LIB OBJECT)

target_sources(OBJECT_LIB PRIVATE
  Lexer.cpp
  LexerStream.cpp
  Instruction.cpp
  Util.cpp
  VMType.cpp
  VMStruct.cpp
)

if( CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(OBJECT_LIB -fsanitize=address)
endif()

target_include_directories(OBJECT_LIB PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE
    main.cpp
)
