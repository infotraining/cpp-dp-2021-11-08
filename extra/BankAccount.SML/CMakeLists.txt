get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME_STR})

# Include directories
set(PROJECT_LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_LIBS_DIR})

# Subdirectories
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

#----------------------------------------
# Application
#----------------------------------------

# Source files
aux_source_directory(./src SRC_LIST)

# Headers
file(GLOB HEADERS_LIST "./src/*.h" "./src/*.hpp")

# Executable
add_executable(${PROJECT_NAME} main.cpp ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LIB})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)