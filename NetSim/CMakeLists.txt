cmake_minimum_required(VERSION 3.17)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion -Werror=switch)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/reports.cpp
        src/simulation.cpp
)

add_executable(NetSim ${SOURCE_FILES} main.cpp)

#Google Test Framework
set(SOURCES_FILES_TESTS
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        )

add_executable(NetSim_tests ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(NetSim_tests PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)
target_link_libraries(NetSim_tests gmock)
target_compile_definitions(NetSim PUBLIC EXERCISE_ID=SIMULATION)
target_compile_definitions(NetSim_tests PUBLIC EXERCISE_ID=SIMULATION)
