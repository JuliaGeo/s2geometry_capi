cmake_minimum_required(VERSION 3.10)
project(s2c_api VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

# Find s2geometry - allow multiple methods
if(NOT DEFINED S2_ROOT)
    # Try to find s2geometry in parent directory first
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../s2geometry")
        set(S2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../s2geometry")
    endif()
endif()

# Find required packages
find_package(Threads REQUIRED)

# Option to build with system s2geometry or local one
option(USE_SYSTEM_S2 "Use system-installed s2geometry" OFF)

if(USE_SYSTEM_S2)
    find_package(s2 REQUIRED)
else()
    # Build s2geometry as part of this project
    if(NOT EXISTS "${S2_ROOT}/CMakeLists.txt")
        message(FATAL_ERROR "s2geometry not found at ${S2_ROOT}. Set S2_ROOT or use USE_SYSTEM_S2=ON")
    endif()
    
    # Add s2geometry as subdirectory
    add_subdirectory(${S2_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/s2geometry)
endif()

# Create s2c library
add_library(s2c SHARED
    src/s2c.cpp
)

target_include_directories(s2c PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(USE_SYSTEM_S2)
    target_link_libraries(s2c PUBLIC s2)
else()
    target_link_libraries(s2c PUBLIC s2)
    target_include_directories(s2c PRIVATE ${S2_ROOT}/src)
endif()

# Set library properties
set_target_properties(s2c PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/s2c.h
)

# Build examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(TARGETS s2c
    EXPORT s2cTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Export targets
install(EXPORT s2cTargets
    FILE s2cTargets.cmake
    NAMESPACE s2c::
    DESTINATION lib/cmake/s2c
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/s2cConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/s2cConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/s2cConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/s2cConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/s2cConfigVersion.cmake"
    DESTINATION lib/cmake/s2c
)