# Simple test runner without GoogleTest dependency
add_executable(test_runner test_runner.c)
target_link_libraries(test_runner s2c m)
target_include_directories(test_runner PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Enable testing
enable_testing()
add_test(NAME s2c_tests COMMAND test_runner)

# Optional: Add GoogleTest-based tests if available
find_package(GTest QUIET)

if(GTest_FOUND)
    message(STATUS "GoogleTest found, building advanced tests")
    
    set(GTEST_SOURCES
        test_basic_types.cc
        test_geometry_types.cc
        test_utilities.cc
    )
    
    foreach(test_source ${GTEST_SOURCES})
        get_filename_component(test_name ${test_source} NAME_WE)
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} 
            s2c
            GTest::gtest
            GTest::gtest_main
            ${CMAKE_THREAD_LIBS_INIT}
        )
        target_include_directories(${test_name} PRIVATE
            ${CMAKE_SOURCE_DIR}/include
        )
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
else()
    message(STATUS "GoogleTest not found, only basic tests will be built")
endif()

# Add a custom target to run all tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)