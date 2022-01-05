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
set(SOURCES src/fs/ParseYahooCsv.cpp src/tests/testPerformance.cpp src/financialProducts/Performance.cpp src/MarketData.cpp src/utils/Date.cpp)
set(HEADERS headers/fs/ParseYahooCsv.hpp headers/financialProducts/Performance.hpp headers/MarketData.hpp headers/utils/Date.hpp)
add_executable(performance ${SOURCES} ${HEADERS})
target_link_libraries(performance ${LIBS})
pnl_add_postbuild(performance)

set(SOURCES src/fs/ParseYahooCsv.cpp src/strategies/simple.cpp src/MarketData.cpp src/utils/Date.cpp src/utils/Utils.cpp)
set(HEADERS headers/fs/ParseYahooCsv.hpp headers/MarketData.hpp headers/utils/Date.hpp headers/utils/Utils.hpp)
add_executable(simple ${SOURCES} ${HEADERS})
target_link_libraries(simple ${LIBS})
pnl_add_postbuild(simple)

#Testing the utils class
set(SOURCES src/utils/Utils.cpp src/tests/TestUtils.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(utils ${SOURCES} ${HEADERS})
target_link_libraries(utils ${LIBS})
pnl_add_postbuild(utils)

set(SOURCES src/utils/Utils.cpp src/tests/TestSize0.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(utilsSize ${SOURCES} ${HEADERS})
target_link_libraries(utilsSize ${LIBS})
pnl_add_postbuild(utilsSize)

set(SOURCES src/utils/Utils.cpp src/tests/TestSizeDifferents.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(utilsdiff ${SOURCES} ${HEADERS})
target_link_libraries(utilsdiff ${LIBS})
pnl_add_postbuild(utilsdiff)

set(SOURCES src/utils/Date.cpp src/tests/testDate.cpp)
set(HEADERS headers/utils/Date.hpp)
add_executable(date ${SOURCES} ${HEADERS})
target_link_libraries(date ${LIBS})
pnl_add_postbuild(date)

set(SOURCES src/MarketData.cpp src/fs/ParseYahooCsv.cpp src/tests/testMarketData.cpp src/utils/Date.cpp)
set(HEADERS headers/MarketData.hpp headers/fs/ParseYahooCsv.hpp headers/utils/Date.hpp)
add_executable(data ${SOURCES} ${HEADERS})
target_link_libraries(data ${LIBS})
pnl_add_postbuild(data)

set(SOURCES src/MarketData.cpp src/fs/ParseYahooCsv.cpp src/tests/testMarketData.cpp)
set(HEADERS headers/MarketData.hpp headers/fs/ParseYahooCsv.hpp)
add_library(datas SHARED ${SOURCES} ${HEADERS})
target_link_libraries(datas ${LIBS})
pnl_add_postbuild(datas)

set(SOURCES src/fs/ParseCsv.cpp src/tests/TestHedge.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)

set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/ProduitDerive.hpp)
add_executable(testVanille ${SOURCES} ${HEADERS})
target_link_libraries(testVanille ${LIBS})
pnl_add_postbuild(testVanille)


#Testing the price calculus
set(SOURCES src/fs/ParseCsv.cpp src/tests/TestPriceCall.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/ProduitDerive.hpp)
add_executable(testPrice ${SOURCES} ${HEADERS})
target_link_libraries(testPrice ${LIBS})
pnl_add_postbuild(testPrice)

set(SOURCES src/fs/ParseCsv.cpp src/tests/TestPriceDateCall.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/ProduitDerive.hpp)
add_executable(testPriceDate ${SOURCES} ${HEADERS})
target_link_libraries(testPriceDate ${LIBS})
pnl_add_postbuild(testPriceDate)

#Testing the simulMarket function
set(SOURCES src/fs/ParseCsv.cpp src/tests/TestSimulMarket.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/ProduitDerive.hpp)
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
