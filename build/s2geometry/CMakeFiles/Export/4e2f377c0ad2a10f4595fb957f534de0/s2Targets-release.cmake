#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "s2::s2" for configuration "Release"
set_property(TARGET s2::s2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(s2::s2 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libs2.0.11.1.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libs2.0.dylib"
  )

list(APPEND _cmake_import_check_targets s2::s2 )
list(APPEND _cmake_import_check_files_for_s2::s2 "${_IMPORT_PREFIX}/lib/libs2.0.11.1.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
