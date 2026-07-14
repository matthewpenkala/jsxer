cmake_minimum_required(VERSION 3.26)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" JSXER_VERSION LIMIT_COUNT 1)
project(Jsxer VERSION "${JSXER_VERSION}" LANGUAGES CXX)

include(CTest)
include(FetchContent)
include(GNUInstallDirs)

option(JSXER_BUILD_CLI "Build the jsxer command-line application" ON)
option(JSXER_BUILD_SHARED "Build the C-compatible shared library" ON)
option(JSXER_WARNINGS_AS_ERRORS "Treat project warnings as errors" OFF)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

# Dependency archives are pinned to immutable commits and verified before use.
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(fmt
    URL https://github.com/fmtlib/fmt/archive/1be298e1bd68957e4cd352e1f676f00e07dcfb57.tar.gz # 12.2.0
    URL_HASH SHA256=d041f836d9b2a3c1c0fe3cce81ffe6a4a257028faa2f4af0dd20d7e58b493766
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(CLI11_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(cli11
    URL https://github.com/CLIUtils/CLI11/archive/37bb6edc5317e99af72ef48405e65d9ca5218861.tar.gz # 2.6.2
    URL_HASH SHA256=62ecd3cd7e8ce36506ea8c19e20554a933c82f45d6a75a39cc8778e0c5170a66
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(fmt cli11)

set(JSXER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(JSXER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

function(jsxer_enable_warnings target)
    if(MSVC)
        # fmt 12 triggers C4127 in MSVC template internals; keep /WX for all
        # actionable project warnings while suppressing that external noise.
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8 /wd4127)
        if(JSXER_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(JSXER_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

function(jsxer_set_output_directories target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    )
endfunction()

file(GLOB_RECURSE JSXER_CORE_HEADERS CONFIGURE_DEPENDS
    "${JSXER_SOURCE_DIR}/jsxer/*.h"
    "${JSXER_SOURCE_DIR}/jsxer/*.hpp"
)
file(GLOB_RECURSE JSXER_CORE_SOURCES CONFIGURE_DEPENDS
    "${JSXER_SOURCE_DIR}/jsxer/*.c"
    "${JSXER_SOURCE_DIR}/jsxer/*.cpp"
)

add_library(jsxer-core STATIC ${JSXER_CORE_HEADERS} ${JSXER_CORE_SOURCES})
add_library(Jsxer::Core ALIAS jsxer-core)
target_compile_features(jsxer-core PUBLIC cxx_std_17)
target_include_directories(jsxer-core PUBLIC
    $<BUILD_INTERFACE:${JSXER_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(jsxer-core PRIVATE fmt::fmt-header-only)
target_compile_definitions(jsxer-core PUBLIC CONFIG_VERSION="${PROJECT_VERSION}")
set_target_properties(jsxer-core PROPERTIES
    OUTPUT_NAME libjsxer
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)
jsxer_enable_warnings(jsxer-core)
jsxer_set_output_directories(jsxer-core)

set(JSXER_INSTALL_TARGETS jsxer-core)

if(JSXER_BUILD_SHARED)
    add_library(jsxer-dll SHARED
        "${JSXER_SOURCE_DIR}/dll/library.cpp"
        "${JSXER_SOURCE_DIR}/dll/library.h"
        "${JSXER_INCLUDE_DIR}/jsxer_c.h"
    )
    add_library(Jsxer::Shared ALIAS jsxer-dll)
    target_compile_features(jsxer-dll PUBLIC cxx_std_17)
    target_include_directories(jsxer-dll PUBLIC
        $<BUILD_INTERFACE:${JSXER_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_link_libraries(jsxer-dll PRIVATE jsxer-core)
    target_compile_definitions(jsxer-dll PRIVATE JSXER_DLL_EXPORTS)
    set_target_properties(jsxer-dll PROPERTIES OUTPUT_NAME lib-jsxer PREFIX "")
    jsxer_enable_warnings(jsxer-dll)
    jsxer_set_output_directories(jsxer-dll)
    list(APPEND JSXER_INSTALL_TARGETS jsxer-dll)
endif()

if(JSXER_BUILD_CLI)
    add_executable(jsxer-cli
        "${JSXER_SOURCE_DIR}/cli/main.cpp"
        "${JSXER_SOURCE_DIR}/cli/utils.h"
    )
    add_executable(Jsxer::CLI ALIAS jsxer-cli)
    target_compile_features(jsxer-cli PRIVATE cxx_std_17)
    target_link_libraries(jsxer-cli PRIVATE jsxer-core CLI11::CLI11)
    set_target_properties(jsxer-cli PROPERTIES OUTPUT_NAME jsxer)
    jsxer_enable_warnings(jsxer-cli)
    jsxer_set_output_directories(jsxer-cli)
    list(APPEND JSXER_INSTALL_TARGETS jsxer-cli)
endif()

if(BUILD_TESTING)
    set(JSXER_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/src")
    set(JSXER_TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/data")

    function(jsxer_add_test test_name source_file)
        string(REPLACE "." "-" target_suffix "${test_name}")
        string(TOLOWER "${target_suffix}" target_suffix)
        set(target_name "jsxer-test-${target_suffix}")
        add_executable(${target_name} "${JSXER_TEST_SOURCE_DIR}/${source_file}")
        target_compile_features(${target_name} PRIVATE cxx_std_17)
        target_compile_definitions(${target_name} PRIVATE JSXER_TEST_DATA_DIR="${JSXER_TEST_DATA_DIR}")
        target_link_libraries(${target_name} PRIVATE jsxer-core)
        jsxer_enable_warnings(${target_name})
        jsxer_set_output_directories(${target_name})
        add_test(NAME "${test_name}" COMMAND ${target_name})
    endfunction()

    jsxer_add_test("CodeGen.ArrayExpression" array-expr.cpp)
    jsxer_add_test("CodeGen.ObjectExpression" obj-expr.cpp)
    jsxer_add_test("CodeGen.MemberExpression" member-expr.cpp)
    jsxer_add_test("CodeGen.ForStatement" for-stmt.cpp)
    jsxer_add_test("CodeGen.NumberFormatting" n2s-formatting.cpp)
    jsxer_add_test("Regression.NumberMember" number-member.cpp)
    jsxer_add_test("Regression.ComplexPrecedence" complex.cpp)
    jsxer_add_test("Regression.StringConcat" string-concat.cpp)
    jsxer_add_test("Regression.SparseArray" sparse-array.cpp)
    jsxer_add_test("Regression.UnicodeIdentifiers" unicode-identifiers.cpp)
    jsxer_add_test("Safety.Core" core-safety.cpp)

    if(JSXER_BUILD_SHARED)
        add_executable(jsxer-test-c-api "${JSXER_TEST_SOURCE_DIR}/c-api.cpp")
        target_compile_features(jsxer-test-c-api PRIVATE cxx_std_17)
        target_link_libraries(jsxer-test-c-api PRIVATE jsxer-dll)
        jsxer_enable_warnings(jsxer-test-c-api)
        jsxer_set_output_directories(jsxer-test-c-api)
        add_test(NAME API.CBoundary COMMAND jsxer-test-c-api)
    endif()

    if(JSXER_BUILD_CLI)
        add_test(NAME CLI.Version COMMAND jsxer-cli --version)
        set_tests_properties(CLI.Version PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

        add_test(NAME CLI.InvalidInput COMMAND jsxer-cli "${JSXER_TEST_DATA_DIR}/invalid.jsxbin")
        set_tests_properties(CLI.InvalidInput PROPERTIES WILL_FAIL TRUE)
    endif()
endif()

install(TARGETS ${JSXER_INSTALL_TARGETS}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES
    "${JSXER_INCLUDE_DIR}/jsxer.h"
    "${JSXER_INCLUDE_DIR}/jsxer_c.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
