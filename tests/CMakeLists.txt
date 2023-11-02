cmake_minimum_required(VERSION 3.15)

project(tests)

set(APP_PATH app_testing)

aux_source_directory(./unit UNIT)
aux_source_directory(./${APP_PATH}/src SRC)

add_executable(unit ${UNIT})
add_executable(app ${SRC})

target_include_directories( app PRIVATE ${INCLUDE_DIR} ${APP_PATH}/include/)
target_include_directories( unit PRIVATE ${INCLUDE_DIR} )

target_link_libraries( unit PRIVATE ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
