
set(LIBRARY_NAME solocraft)

#Add PCH Files
set(solocraft_PCH "${CMAKE_CURRENT_SOURCE_DIR}/solocraftpch.h")

#Base Files
file(GLOB SoloCraft_Source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    source_group("SoloCraft" FILES ${SoloCraft_Source})

#Define base source library
set(LIBRARY_SRCS
    ${SoloCraft_Source}
    ${solocraft_PCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/solocraftpch.cpp
)

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

if(PCH)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
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

target_include_directories(${LIBRARY_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Solocraft")
set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER "Modules")

# Install config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/SoloCraft.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist)
if (NOT CONF_INSTALL_DIR)
  set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SoloCraft.conf.dist DESTINATION ${CONF_INSTALL_DIR})

# Generate PCH
if(PCH)
  target_precompile_headers(${LIBRARY_NAME} PRIVATE ${solocraft_PCH})
endif()
