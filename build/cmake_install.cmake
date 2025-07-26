# Install script for directory: /Users/anshul/.julia/dev/geo/s2c/s2c_api

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/libs2c.1.0.0.dylib"
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/libs2c.1.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2c.1.0.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2c.1.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry"
        -delete_rpath "/opt/homebrew/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/libs2c.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/include/s2c.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c/s2cTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c/s2cTargets.cmake"
         "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/CMakeFiles/Export/b62f78cb25791950e1ad7bb356b9e9de/s2cTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c/s2cTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c/s2cTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/CMakeFiles/Export/b62f78cb25791950e1ad7bb356b9e9de/s2cTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/CMakeFiles/Export/b62f78cb25791950e1ad7bb356b9e9de/s2cTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/s2c" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2cConfig.cmake"
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2cConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/cmake_install.cmake")
  include("/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/examples/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
