
set(LIBRARY_NAME solocraft)

#Base Files
file(GLOB SoloCraft_Source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    source_group("SoloCraft" FILES ${SoloCraft_Source})

#Define base source library
set(LIBRARY_SRCS ${SoloCraft_Source})

include_directories(${CMAKE_SOURCE_DIR}/src/modules/modules/src)
add_definitions(-DENABLE_MODULES)

if (BUILD_SOLOCRAFT)
    add_definitions(-DENABLE_SOLOCRAFT)
endif()

# Define Expansion
add_definitions(-DSOLOCRAFT_CLASSIC)
if ( ${CMAKE_PROJECT_NAME} MATCHES "TBC")
  add_definitions(-DSOLOCRAFT_TBC)
endif()
if ( ${CMAKE_PROJECT_NAME} MATCHES "WoTLK")
  add_definitions(-DSOLOCRAFT_TBC)
  add_definitions(-DSOLOCRAFT_WoTLK)
endif()

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

target_link_libraries(${LIBRARY_NAME}
  modules
  shared
  RecastNavigation::Detour
)

if(UNIX)
  # Both systems don't have libdl and don't need them
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(${LIBRARY_NAME} dl)
  endif()
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Solocraft")
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER "Modules")

# Install config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/SoloCraft.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist)
if (NOT CONF_INSTALL_DIR)
  set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist DESTINATION ${CONF_INSTALL_DIR})
