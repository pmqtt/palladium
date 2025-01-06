
target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_library(OBJECT_LIB OBJECT)

file(GLOB ALL_OBJ *.cpp ast/*.cpp)
list(REMOVE_ITEM ALL_OBJ ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_sources(OBJECT_LIB PRIVATE ${ALL_OBJ})

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  target_link_libraries(OBJECT_LIB -fsanitize=address)
endif()

target_include_directories(OBJECT_LIB PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/ast)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE
    main.cpp
)
