cmake_minimum_required(VERSION 3.13)
project(PriceBSCall CXX)

set(CMAKE_CXX_STANDARD 17)
# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Testing the parser
set(SOURCES src/ParseYahooCsv.cpp src/testPerformance.cpp src/Performance.cpp src/MarketData.cpp src/Date.cpp)
set(HEADERS src/ParseYahooCsv.hpp src/Performance.hpp src/MarketData.hpp src/Date.cpp)
add_executable(performance ${SOURCES} ${HEADERS})
target_link_libraries(performance ${LIBS})
pnl_add_postbuild(performance)

#Testing the utils class
set(SOURCES src/Utils.cpp src/TestUtils.cpp)
set(HEADERS src/Utils.hpp)
add_executable(utils ${SOURCES} ${HEADERS})
target_link_libraries(utils ${LIBS})
pnl_add_postbuild(utils)

set(SOURCES src/Utils.cpp src/TestSize0.cpp)
set(HEADERS src/Utils.hpp)
add_executable(utilsSize ${SOURCES} ${HEADERS})
target_link_libraries(utilsSize ${LIBS})
pnl_add_postbuild(utilsSize)

set(SOURCES src/Utils.cpp src/TestSizeDifferents.cpp)
set(HEADERS src/Utils.hpp)
add_executable(utilsdiff ${SOURCES} ${HEADERS})
target_link_libraries(utilsdiff ${LIBS})
pnl_add_postbuild(utilsdiff)

set(SOURCES src/Date.cpp src/testDate.cpp)
set(HEADERS src/Date.hpp)
add_executable(date ${SOURCES} ${HEADERS})
target_link_libraries(date ${LIBS})
pnl_add_postbuild(date)

set(SOURCES src/MarketData.cpp src/ParseYahooCsv.cpp src/testMarketData.cpp src/Date.cpp)
set(HEADERS src/MarketData.hpp src/ParseYahooCsv.hpp src/Date.hpp)
add_executable(data ${SOURCES} ${HEADERS})
target_link_libraries(data ${LIBS})
pnl_add_postbuild(data)

set(SOURCES src/MarketData.cpp src/ParseYahooCsv.cpp src/testMarketData.cpp)
set(HEADERS src/MarketData.hpp src/ParseYahooCsv.hpp)
add_library(datas SHARED ${SOURCES} ${HEADERS})
target_link_libraries(datas ${LIBS})
pnl_add_postbuild(datas)


# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
