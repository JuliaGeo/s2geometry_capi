# Example programs
add_executable(polygon_operations polygon_operations.c)
target_link_libraries(polygon_operations s2c m)

add_executable(simple_polygon_test simple_polygon_test.c)
target_link_libraries(simple_polygon_test s2c m)

# Add more examples here as needed