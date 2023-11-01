cmake_minimum_required(VERSION 3.15)

project(tests)

aux_source_directory(./unit UNIT)
add_executable(unit ${UNIT})

target_include_directories( unit PRIVATE ${INCLUDE_DIR} )

target_link_libraries( unit PRIVATE ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
