message("Building test files...")
include_directories(${CMAKE_SOURCE_DIR}/test/doctest)
set(STATIC_TEST_PATH ${CMAKE_SOURCE_DIR}/test)

file(GLOB STATIC_TEST_FILES ${STATIC_TEST_PATH}/*.cc ${STATIC_TEST_PATH}/*.cpp)
foreach(TEST_FILE ${STATIC_TEST_FILES})
    get_filename_component(EXECUTABLE_NAME ${TEST_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${TEST_FILE})
endforeach() 