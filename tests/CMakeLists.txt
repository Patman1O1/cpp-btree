#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: cpp_btree_test
# Alias: collections::cpp_btree_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::cpp_btree_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(cpp_btree_test)

# Target alias
add_executable(collections::cpp_btree_test ALIAS cpp_btree_test)

# Target sources
target_sources(cpp_btree_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_btree_test.cpp
)

# Target dependencies
target_link_libraries(cpp_btree_test
    PRIVATE
        collections::cpp_btree_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(cpp_btree_test)