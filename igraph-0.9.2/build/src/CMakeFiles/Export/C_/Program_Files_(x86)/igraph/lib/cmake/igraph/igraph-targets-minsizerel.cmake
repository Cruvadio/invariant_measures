#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "igraph::igraph" for configuration "MinSizeRel"
set_property(TARGET igraph::igraph APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(igraph::igraph PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C;CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/igraph.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS igraph::igraph )
list(APPEND _IMPORT_CHECK_FILES_FOR_igraph::igraph "${_IMPORT_PREFIX}/lib/igraph.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
