cmake_minimum_required(VERSION 3.16)
project(arraylib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Динамическая библиотека ---
add_library(arraylib SHARED src/arraylib.cpp)

target_include_directories(arraylib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(arraylib PRIVATE ARRAYLIB_BUILD)

if (MSVC)
    target_compile_options(arraylib PRIVATE /W4)
else()
    target_compile_options(arraylib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Демонстрационная программа ---
add_executable(app main.cpp)
target_link_libraries(app PRIVATE arraylib)
set_target_properties(app PROPERTIES BUILD_RPATH "$ORIGIN")

set_target_properties(arraylib app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# --- Тесты ---
include(CTest)
if (BUILD_TESTING)
    # Unit-тесты
    add_executable(test_unit tests/test_unit.cpp)
    target_link_libraries(test_unit PRIVATE arraylib)
    target_compile_options(test_unit PRIVATE -UNDEBUG)
    # Используем TARGET_FILE, чтобы ctest знал точный путь
    add_test(NAME unit COMMAND $<TARGET_FILE:test_unit>)

    # Интеграционные тесты
    add_executable(test_integration tests/test_integration.cpp)
    target_link_libraries(test_integration PRIVATE arraylib)
    target_compile_options(test_integration PRIVATE -UNDEBUG)
    add_test(NAME integration COMMAND $<TARGET_FILE:test_integration>)
endif()

# --- Пример по варианту ---
add_executable(variant_01 examples/variant_01.cpp)
target_link_libraries(variant_01 PRIVATE arraylib)