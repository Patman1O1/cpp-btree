#-----------------------------------------------------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------------------------------------------------
include(GoogleTest)
enable_testing()

#-----------------------------------------------------------------------------------------------------------------------
# Target: btree_test
# Alias: collections::btree_test
# Type: Executable
# Dependencies: GTest::gtest_main, GTest::gmock_main, collections::btree_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_executable(btree_test)

# Target alias
add_executable(collections::btree_test ALIAS btree_test)

# Target sources
target_sources(btree_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/btree_test.cpp
)

# Target dependencies
target_link_libraries(btree_test
    PRIVATE
        collections::btree_headers
        GTest::gtest_main
        GTest::gmock_main
)

# Test discovery
gtest_discover_tests(btree_test)
