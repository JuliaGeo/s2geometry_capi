# Install script for directory: /Users/anshul/.julia/dev/geo/s2c/s2geometry

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/_fp_contract_off.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/encoded_s2cell_id_vector.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/encoded_s2point_vector.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/encoded_s2shape_index.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/encoded_string_vector.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/encoded_uint_vector.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/id_set_lexicon.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/mutable_s2shape_index.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/r1interval.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/r2.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/r2rect.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s1angle.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s1chord_angle.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s1interval.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2boolean_operation.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2buffer_operation.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builder.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builder_graph.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builder_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_closed_set_normalizer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_find_polygon_degeneracies.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_get_snapped_winding_delta.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_graph_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_lax_polygon_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_lax_polyline_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_s2point_vector_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_s2polygon_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_s2polyline_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_s2polyline_vector_layer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_snap_functions.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2builderutil_testing.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cap.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_id.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_index.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_iterator.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_iterator_join.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_range_iterator.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2cell_union.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2centroids.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_cell_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_cell_query_base.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_edge_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_edge_query_base.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_point_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2closest_point_query_base.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2coder.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2contains_point_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2contains_vertex_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2convex_hull_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2coords_internal.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2coords.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2crossing_edge_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2debug.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2distance_target.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2earth.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_clipping.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_crosser.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_crossings.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_crossings_internal.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_distances.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_tessellator.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2edge_vector_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2error.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2furthest_edge_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2hausdorff_distance_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2latlng.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2latlng_rect.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2latlng_rect_bounder.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2lax_loop_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2lax_polygon_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2lax_polyline_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2loop.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2loop_measures.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2measures.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2memory_tracker.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2metrics.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2max_distance_targets.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2min_distance_targets.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2padded_cell.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point_vector_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point_compression.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point_index.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point_region.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2point_span.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2pointutil.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2polygon.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2polyline.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2polyline_alignment.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2polyline_measures.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2polyline_simplifier.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2predicates.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2predicates_internal.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2projections.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2r2rect.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2region.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2region_term_indexer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2region_coverer.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2region_intersection.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2region_union.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape_index.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape_index_buffered_region.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape_index_region.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape_measures.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shape_nesting_query.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_build_polygon_boundaries.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_coding.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_contains_brute_force.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_conversion.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_count_edges.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_edge_iterator.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_get_reference_point.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_shape_edge.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_shape_edge_id.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_testing.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2shapeutil_visit_crossing_edge_pairs.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2testing.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2text_format.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2wedge_relations.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2winding_operation.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/s2wrapped_shape.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/sequence_lexicon.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/thread_testing.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/value_lexicon.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/base" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/casts.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/commandlineflags.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/commandlineflags_declare.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/integral_types.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/log_severity.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/logging.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/port.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/base/spinlock.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/testing" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/testing/gtest_prod.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/bitmap" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/bitmap/bitmap.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/bits" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/bits/bits.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/coding" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/coding/coder.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/coding/varint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/endian" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/endian/endian.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/gtl" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/gtl/compact_array.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/gtl/container_logging.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/gtl/dense_hash_set.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/gtl/densehashtable.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/gtl/hashtable_common.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/hash" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/hash/mix.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/math" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/math/mathutil.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/math/matrix3x3.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/math/vector.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/math/exactfloat" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/math/exactfloat/exactfloat.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/s2/util/units" TYPE FILE FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/units/length-units.h"
    "/Users/anshul/.julia/dev/geo/s2c/s2geometry/src/s2/util/units/physical-units.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.0.11.1.dylib"
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.0.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2.0.11.1.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2.0.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/opt/homebrew/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.0.11.1.dylib"
    "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.0.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2.0.11.1.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libs2.0.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/opt/homebrew/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/libs2.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/s2/s2Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/s2/s2Targets.cmake"
         "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/CMakeFiles/Export/4e2f377c0ad2a10f4595fb957f534de0/s2Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/s2/s2Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/s2/s2Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/s2" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/CMakeFiles/Export/4e2f377c0ad2a10f4595fb957f534de0/s2Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/s2" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/CMakeFiles/Export/4e2f377c0ad2a10f4595fb957f534de0/s2Targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/s2" TYPE FILE FILES "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/s2Config.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/anshul/.julia/dev/geo/s2c/s2c_api/build/s2geometry/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
