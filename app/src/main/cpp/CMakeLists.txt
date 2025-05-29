cmake_minimum_required(VERSION 3.22.1)

project(SelfApp)

# Source files
set(SOURCE_FILES
    native-lib.cpp
)

# Define library name explicitly
set(LIB_NAME native-lib)

# Create shared library
add_library(${LIB_NAME} SHARED ${SOURCE_FILES})

# Include headers (current dir)
target_include_directories(${LIB_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Find Android log library
find_library(log-lib log)

# Find libcurl using prefab (via Gradle)
find_package(curl REQUIRED CONFIG)

# Link libraries: curl and Android log
target_link_libraries(
    ${LIB_NAME}      # Use variable, not hardcoded name
    curl::curl
    ${log-lib}
)