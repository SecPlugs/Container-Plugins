cmake_minimum_required(VERSION 3.17)
project(secplugs_common)

set(CMAKE_CXX_STANDARD 17)
find_package(Poco REQUIRED Foundation Net NetSSL JSON Util)
add_library(secplugs-rest SHARED
        lib/rest/restclient.cpp lib/rest/restclient.h)
add_library(secplugs-common SHARED
        lib/filewatcher/filewatcher.cpp lib/filewatcher/filewatcher.h
        lib/utils/common.cpp lib/utils/common.h
        lib/config/configloader.cpp lib/config/configloader.h)

target_link_libraries(secplugs-common secplugs-rest ${Poco_LIBRARIES} stdc++fs)
target_include_directories(secplugs-common PUBLIC lib/)
target_include_directories(secplugs-rest PUBLIC lib/)


file(GLOB PLUGINSRC bin/*.cpp)
add_executable(dockerplugin ${PLUGINSRC})
target_link_libraries(dockerplugin PUBLIC secplugs-common)
target_link_directories(dockerplugin INTERFACE ${PROJECT_SOURCE_DIR}/cmake-build-debug)
target_include_directories(dockerplugin PUBLIC ${PROJECT_SOURCE_DIR}/lib)

add_custom_target(deb ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../packaging/debian/
        COMMAND echo ${PROJECT_SOURCE_DIR}/../packaging/debian/
        COMMAND cp ${PROJECT_SOURCE_DIR}/cmake-build-release/libsecplugs-rest.so secplugs-linux-oas-plugin/usr/local/lib
        COMMAND cp ${PROJECT_SOURCE_DIR}/cmake-build-release/libsecplugs-common.so secplugs-linux-oas-plugin/usr/local/lib
        COMMAND cp ${PROJECT_SOURCE_DIR}/cmake-build-release/dockerplugin secplugs-linux-oas-plugin/usr/local/bin
        COMMAND dpkg -b secplugs-linux-oas-plugin

        )
