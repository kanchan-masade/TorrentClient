cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(TorrentClient C CXX ASM)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" "${CMAKE_MODULE_PATH}")

find_program(
	MAKE
	NAMES "make${CMAKE_EXECUTABLE_SUFFIX}" "make${CMAKE_EXECUTABLE_SUFFIX}"
	DOC "Location of the make"
)
if(NOT MAKE)
	message(FATAL_ERROR "make not found")
endif()
message(STATUS "Check for MAKE: found ${MAKE}")


include(FindUBSan)
include(FindTSan)
include(FindASan)
include(FindMSan)

set(GFLAGS_ROOT_DIR "")
include(FindGFlags "${CMAKE_CURRENT_SOURCE_DIR}/install")

# so that "make" will echo the command line for easy debugging
# set(CMAKE_VERBOSE_MAKEFILE on)

# For Sanitizer builds, set it to ASan/MSan/TSan.
# See the CMake directory for sanitizer cmake files
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "No build type selected, default to Debug")
	message(STATUS "To change pass -DCMAKE_BUILD_TYPE on command line")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_COLOR_MAKEFILE)

set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(STATUS "In-source-builds are not allowed")
	message(STATUS "Clean your source directory (e.g. delete the CMakeCache.txt file)")
	message(FATAL_ERROR "Please create a separate build directory and call CMake again")
endif(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})

# all headers specified wrt the top-level src dir
include_directories(src/include)
include_directories(src/)
include_directories(thirdparty/bencode.hpp/include/)

set(CXX_STANDARD "c++17")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CXX_STANDARD} -Wnon-virtual-dtor -Woverloaded-virtual -DBOOST_LOG_DYN_LINK")

add_compile_options(
	-Wall
	-Wextra
	-Werror
	-Wstrict-aliasing
	-Wno-unused-parameter
	-Wno-missing-field-initializers
	-Wchar-subscripts
	-Wpointer-arith
	-Wformat
	-Wformat-security
	-Werror=format-security
	-fstack-protector-all
	-fPIE
	-fpie
	-fPIC
	-fpic
	-pipe
	-fdata-sections
	-ffunction-sections
	-g
	-ggdb
)

# Include cppcheck target.  must come after compiler flags defined
include(cppcheck)
include(clang-format)

enable_testing()

set(LibCryptopp
	${CMAKE_SOURCE_DIR}/thirdparty/cryptopp/libcryptopp.a
)

add_custom_command(
	OUTPUT ${LibCryptopp}
	COMMAND ${MAKE} -C ${CMAKE_SOURCE_DIR}/thirdparty/cryptopp/
	COMMENT "Building cryptopp.... "
)

add_custom_target(
	TargetLibCryptoPP
	DEPENDS ${LibCryptopp}
)

set(ENV{BUILD_CPR_TESTS} OFF)
set(ENV{CPR_CURL_NOSIGNAL} ON)
set(ENV{USE_SYSTEM_GTEST} ON)
add_subdirectory(thirdparty/cpr)

include_directories(${CPR_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/)

add_subdirectory(src)
