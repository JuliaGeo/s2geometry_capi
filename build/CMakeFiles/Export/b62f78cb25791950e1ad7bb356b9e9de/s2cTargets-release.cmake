#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "s2c::s2c" for configuration "Release"
set_property(TARGET s2c::s2c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(s2c::s2c PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libs2c.1.0.0.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libs2c.1.dylib"
  )

list(APPEND _cmake_import_check_targets s2c::s2c )
list(APPEND _cmake_import_check_files_for_s2c::s2c "${_IMPORT_PREFIX}/lib/libs2c.1.0.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
