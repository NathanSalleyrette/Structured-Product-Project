cmake_minimum_required(VERSION 3.5)
project(PriceBSCall CXX)

set(CMAKE_CXX_STANDARD 17)


# A decommenter si on veut enlever certains flag
#unset (CMAKE_CXX_FLAGS CACHE)

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



include_directories(headers)

set(LOGGER headers/spdlog/log.hpp src/spdlog/log.cpp headers/spdlog/spdlog.h)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Testing the logger
set(SOURCES src/spdlog/log.cpp src/tests/testLogger.cpp)
set(HEADERS headers/spdlog/log.hpp)
add_executable(testLogger ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testLogger ${LIBS})
pnl_add_postbuild(testLogger)

set (SOURCES src/tests/testWriting.cpp)
add_executable(testWrite ${SOURCES})
target_link_libraries(testWrite ${LIBS})
pnl_add_postbuild(testWrite)

# Testing the parser
set(SOURCES src/fs/ParseYahooCsv.cpp src/tests/testPerformance.cpp src/financialProducts/Performance.cpp src/utils/MarketData.cpp src/utils/Date.cpp)
set(HEADERS headers/fs/ParseYahooCsv.hpp headers/financialProducts/Performance.hpp headers/utils/MarketData.hpp headers/utils/Date.hpp)
add_executable(testPerformance ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testPerformance ${LIBS})
pnl_add_postbuild(testPerformance)


set(SOURCES src/fs/ParseYahooCsv.cpp src/strategies/simple.cpp src/utils/MarketData.cpp src/utils/Date.cpp src/utils/Utils.cpp src/models/BlackScholesModel.cpp src/utils/Date.cpp src/montecarlo/MonteCarlo.cpp src/financialProducts/Performance.cpp)
set(HEADERS headers/fs/ParseYahooCsv.hpp headers/utils/MarketData.hpp headers/utils/Date.hpp headers/utils/Utils.hpp headers/models/BlackScholesModel.hpp headers/utils/Date.hpp headers/montecarlo/MonteCarlo.hpp headers/financialProducts/Performance.hpp)
add_executable(simple ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(simple ${LIBS})
pnl_add_postbuild(simple)

set(SOURCES src/fs/ParseYahooCsv.cpp src/strategies/simpleBis.cpp src/utils/MarketData.cpp src/utils/Date.cpp src/utils/Utils.cpp src/models/BlackScholesModel.cpp src/utils/Date.cpp src/montecarlo/MonteCarlo.cpp src/financialProducts/Performance.cpp)
set(HEADERS headers/fs/ParseYahooCsv.hpp headers/utils/MarketData.hpp headers/utils/Date.hpp headers/utils/Utils.hpp headers/models/BlackScholesModel.hpp headers/utils/Date.hpp headers/montecarlo/MonteCarlo.hpp headers/financialProducts/Performance.hpp)
add_executable(simpleBis ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(simpleBis ${LIBS})
pnl_add_postbuild(simpleBis)

#Testing the utils class
set(SOURCES src/utils/Utils.cpp src/tests/TestUtils.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(testUtils ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testUtils ${LIBS})
pnl_add_postbuild(testUtils)

set(SOURCES src/utils/Utils.cpp src/tests/TestSize0.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(testSize0 ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testSize0 ${LIBS})
pnl_add_postbuild(testSize0)

set(SOURCES src/utils/Utils.cpp src/tests/TestSizeDifferents.cpp)
set(HEADERS headers/utils/Utils.hpp)
add_executable(testSizeDifferents ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testSizeDifferents ${LIBS})
pnl_add_postbuild(testSizeDifferents)

#Testing printresults

set(SOURCES src/fs/ParseCsv.cpp src/tests/testResultsPrintVanilla.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testVanilleprint ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testVanilleprint ${LIBS})
pnl_add_postbuild(testVanilleprint)

set(SOURCES src/fs/ParseCsv.cpp src/tests/testVanillePrintDelta.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testVanillePrintDelta ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testVanillePrintDelta ${LIBS})
pnl_add_postbuild(testVanillePrintDelta)

set(SOURCES src/fs/ParseCsv.cpp src/tests/TestVanillaCallPrixent.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(TestVanillaCallPrixent ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(TestVanillaCallPrixent ${LIBS})
pnl_add_postbuild(TestVanillaCallPrixent)

set(SOURCES src/fs/ParseCsv.cpp src/tests/TestPrintDeltaEnt.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(TestPrintDeltaEnt ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(TestPrintDeltaEnt ${LIBS})
pnl_add_postbuild(TestPrintDeltaEnt)

set(SOURCES src/fs/ParseCsv.cpp src/tests/TestPrintPnl.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(TestPrintPnl ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(TestPrintPnl ${LIBS})
pnl_add_postbuild(TestPrintPnl)

# set(SOURCES src/utils/MarketData.cpp src/fs/ParseYahooCsv.cpp)
# set(HEADERS headers/utils/MarketData.hpp headers/fs/ParseYahooCsv.hpp)
# add_library(datas SHARED ${SOURCES} ${HEADERS} ${LOGGER})
# target_link_libraries(datas ${LIBS})
# pnl_add_postbuild(datas)

set(SOURCES src/fs/ParseCsv.cpp src/tests/oldTests/TestHedge.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testVanille ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testVanille ${LIBS})
pnl_add_postbuild(testVanille)


#Testing the price calculus
set(SOURCES src/fs/ParseCsv.cpp src/tests/oldTests/TestPriceCall.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testPrice ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testPrice ${LIBS})
pnl_add_postbuild(testPrice)

set(SOURCES src/fs/ParseCsv.cpp src/tests/oldTests/TestPriceDateCall.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testPriceDate ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testPriceDate ${LIBS})
pnl_add_postbuild(testPriceDate)

#Testing the simulMarket function
set(SOURCES src/fs/ParseCsv.cpp src/tests/TestSimulMarket.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testSimulMarket ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testSimulMarket ${LIBS})
pnl_add_postbuild(testSimulMarket)

#GOOGLETEST
add_executable(googletest src/tests/googletest.cpp)
target_link_libraries(googletest gtest_main)
add_test(NAME googletest_test COMMAND googletest)

set(SOURCES src/utils/Date.cpp src/tests/testDate.cpp)
set(HEADERS headers/utils/Date.hpp)
add_executable(testDate ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testDate gtest_main ${LIBS})
pnl_add_postbuild(date)
add_test(NAME date_test COMMAND testDate)

set(SOURCES src/utils/MarketData.cpp src/fs/ParseYahooCsv.cpp src/tests/testMarketData.cpp src/utils/Date.cpp)
set(HEADERS headers/utils/MarketData.hpp headers/fs/ParseYahooCsv.hpp headers/utils/Date.hpp)
add_executable(testMarketData  ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testMarketData gtest_main ${LIBS})
pnl_add_postbuild(data)
add_test(NAME marketdata_test COMMAND testMarketData)


set(SOURCES src/fs/ParseCsv.cpp src/tests/testCallVanille.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testVanilleGtest ${SOURCES} ${HEADERS} ${LOGGER})
target_link_libraries(testVanilleGtest gtest_main ${LIBS})
pnl_add_postbuild(testVanilleGtest)
add_test(NAME vanille_test COMMAND testVanilleGtest)

set(SOURCES src/fs/ParseCsv.cpp src/tests/testCallHEqualN.cpp src/montecarlo/MonteCarlo.cpp src/models/BlackScholesModel.cpp src/financialProducts/VanillaCall.cpp)
set(HEADERS headers/fs/ParseCsv.hpp headers/montecarlo/MonteCarlo.hpp headers/models/BlackScholesModel.hpp headers/financialProducts/VanillaCall.hpp headers/financialProducts/Derivative.hpp)
add_executable(testVanilleHEqualN ${SOURCES} ${HEADERS})
target_link_libraries(testVanilleHEqualN gtest_main ${LIBS})
pnl_add_postbuild(testVanilleHEqualN)
add_test(NAME vanilleHEqualN_test COMMAND testVanilleHEqualN)


# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
