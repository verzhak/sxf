
#
# Параметры сборки:
#
# CMAKE_BUILD_TYPE=Debug или CMAKE_BUILD_TYPE=Release (по умолчанию)
#

cmake_minimum_required(VERSION 2.8)

project(sxf)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PC_ICONV QUIET iconv)
pkg_check_modules(PC_AMV QUIET amv)

string(REPLACE ";" " " PC_AMV_CFLAGS " ${PC_AMV_CFLAGS}")
set(CMAKE_CXX_FLAGS "${PC_ICONV_CXXFLAGS} ${CMAKE_CXX_FLAGS} ${PC_AMV_CFLAGS} -Wall -pipe -std=gnu++11")
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)

	set(CMAKE_BUILD_TYPE Debug)

endif()

set(sources lib/sxf.cpp lib/sxf_map.cpp lib/sxf_height_map.cpp lib/file.cpp lib/codepage.cpp lib/map.cpp lib/point.cpp)

add_library(sxf SHARED ${sources})
target_link_libraries(sxf m ${PC_AMV_LIBRARIES} ${PC_ICONV_LIBRARIES})

add_executable(sxf_test test.cpp)
target_link_libraries(sxf_test sxf ${PC_AMV_LIBRARIES})

configure_file(sxf.pc.in sxf.pc)
install(TARGETS sxf DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp" PATTERN "build" EXCLUDE PATTERN ".git" EXCLUDE PATTERN "lib" EXCLUDE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sxf.pc DESTINATION lib/pkgconfig)

