set(PROJECT_TESTS ${PROJECT_ID}_tests)
message(STATUS "PROJECT_TESTS is: " ${PROJECT_TESTS})

project(${PROJECT_TESTS} CXX)

file(GLOB TEST_SOURCES *_tests.cpp *_test.cpp)

add_executable(${PROJECT_TESTS} ${TEST_SOURCES})
target_compile_features(${PROJECT_TESTS} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_TESTS} PUBLIC ${PROJECT_LIB})
  
add_test(AllTestsInMain ${PROJECT_TESTS})
