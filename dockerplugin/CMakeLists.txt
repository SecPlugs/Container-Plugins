cmake_minimum_required(VERSION 3.17)
project(secplugs_common)

set(CMAKE_CXX_STANDARD 17)
find_package(Poco REQUIRED Foundation Net NetSSL JSON Util)
add_library(secplugs-common SHARED
        lib/filewatcher/filewatcher.cpp lib/filewatcher/filewatcher.h
        lib/rest/restclient.cpp lib/rest/restclient.h
        lib/utils/common.cpp lib/utils/common.h
        lib/config/configloader.cpp lib/config/configloader.h)

target_link_libraries(secplugs-common ${Poco_LIBRARIES} stdc++fs)
target_include_directories(secplugs-common PUBLIC lib/)

file(GLOB PLUGINSRC bin/*.cpp)
add_executable(dockerplugin ${PLUGINSRC})
target_link_libraries(dockerplugin PUBLIC secplugs-common)
target_link_directories(dockerplugin INTERFACE ${PROJECT_SOURCE_DIR}/cmake-build-debug)
target_include_directories(dockerplugin PUBLIC ${PROJECT_SOURCE_DIR}/lib)


file(COPY tests/data DESTINATION ${PROJECT_SOURCE_DIR}/cmake-build-debug/)

add_executable(configloadertest tests/configloadertest.cpp)
target_link_directories(configloadertest INTERFACE ${PROJECT_SOURCE_DIR}/cmake-build-debug)
target_link_libraries(configloadertest PUBLIC secplugs-common)
target_include_directories(configloadertest PUBLIC ${PROJECT_SOURCE_DIR}/lib)
