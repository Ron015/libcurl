cmake_minimum_required(VERSION 3.22.1)
project("curltest")

add_library(curl SHARED IMPORTED)
set_target_properties(curl PROPERTIES IMPORTED_LOCATION
${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libcurl.so)

add_library(native-lib SHARED native-lib.cpp)

find_library(log-lib log)

target_link_libraries(native-lib curl ${log-lib})