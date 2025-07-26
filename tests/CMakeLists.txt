# Simple test runner without GoogleTest dependency
add_executable(test_runner test_runner.c)
target_link_libraries(test_runner s2c m)
target_include_directories(test_runner PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Additional C test executables
add_executable(test_distance test_distance.c)
target_link_libraries(test_distance s2c m)
target_include_directories(test_distance PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_cells test_cells.c)
target_link_libraries(test_cells s2c m)
target_include_directories(test_cells PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_earth test_earth.c)
target_link_libraries(test_earth s2c m)
target_include_directories(test_earth PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_loops test_loops.c)
target_link_libraries(test_loops s2c m)
target_include_directories(test_loops PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_s2_utils test_s2_utils.c)
target_link_libraries(test_s2_utils s2c m)
target_include_directories(test_s2_utils PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_cellunion test_cellunion.c)
target_link_libraries(test_cellunion s2c m)
target_include_directories(test_cellunion PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_angles test_angles.c)
target_link_libraries(test_angles s2c m)
target_include_directories(test_angles PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_latlngrect test_latlngrect.c)
target_link_libraries(test_latlngrect s2c m)
target_include_directories(test_latlngrect PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_regioncoverer test_regioncoverer.c)
target_link_libraries(test_regioncoverer s2c m)
target_include_directories(test_regioncoverer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_shape_index test_shape_index.c)
target_link_libraries(test_shape_index s2c m)
target_include_directories(test_shape_index PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(test_boolean_operations test_boolean_operations.c)
target_link_libraries(test_boolean_operations s2c m)
target_include_directories(test_boolean_operations PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Enable testing
enable_testing()
add_test(NAME s2c_tests COMMAND test_runner)
add_test(NAME s2c_distance_tests COMMAND test_distance)
add_test(NAME s2c_cell_tests COMMAND test_cells)
add_test(NAME s2c_earth_tests COMMAND test_earth)
add_test(NAME s2c_loop_tests COMMAND test_loops)
add_test(NAME s2c_utils_tests COMMAND test_s2_utils)
add_test(NAME s2c_cellunion_tests COMMAND test_cellunion)
add_test(NAME s2c_angle_tests COMMAND test_angles)
add_test(NAME s2c_latlngrect_tests COMMAND test_latlngrect)
add_test(NAME s2c_regioncoverer_tests COMMAND test_regioncoverer)
add_test(NAME s2c_shape_index_tests COMMAND test_shape_index)
add_test(NAME s2c_boolean_operations_tests COMMAND test_boolean_operations)

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