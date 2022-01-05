cmake_minimum_required(VERSION 2.8)
project(PriceBSCall CXX)

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
set(SOURCES src/ParseCsv.cpp src/TestHedge.cpp src/MonteCarlo.cpp src/BlackScholesModel.cpp src/VanillaCall.cpp)

set(HEADERS src/ParseCsv.hpp src/MonteCarlo.hpp src/BlackScholesModel.hpp src/VanillaCall.hpp src/ProduitDerive.hpp)
add_executable(testVanille ${SOURCES} ${HEADERS})
target_link_libraries(testVanille ${LIBS})
pnl_add_postbuild(testVanille)


#Testing the price calculus
set(SOURCES src/ParseCsv.cpp src/TestPriceCall.cpp src/MonteCarlo.cpp src/BlackScholesModel.cpp src/VanillaCall.cpp)
set(HEADERS src/ParseCsv.hpp src/MonteCarlo.hpp src/BlackScholesModel.hpp src/VanillaCall.hpp src/ProduitDerive.hpp)
add_executable(testPrice ${SOURCES} ${HEADERS})
target_link_libraries(testPrice ${LIBS})
pnl_add_postbuild(testPrice)

set(SOURCES src/ParseCsv.cpp src/TestPriceDateCall.cpp src/MonteCarlo.cpp src/BlackScholesModel.cpp src/VanillaCall.cpp)
set(HEADERS src/ParseCsv.hpp src/MonteCarlo.hpp src/BlackScholesModel.hpp src/VanillaCall.hpp src/ProduitDerive.hpp)
add_executable(testPriceDate ${SOURCES} ${HEADERS})
target_link_libraries(testPriceDate ${LIBS})
pnl_add_postbuild(testPriceDate)

#Testing the simulMarket function
set(SOURCES src/ParseCsv.cpp src/TestSimulMarket.cpp src/MonteCarlo.cpp src/BlackScholesModel.cpp src/VanillaCall.cpp)
set(HEADERS src/ParseCsv.hpp src/MonteCarlo.hpp src/BlackScholesModel.hpp src/VanillaCall.hpp src/ProduitDerive.hpp)
add_executable(testSimulMarket ${SOURCES} ${HEADERS})
target_link_libraries(testSimulMarket ${LIBS})
pnl_add_postbuild(testSimulMarket)

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
