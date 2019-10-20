cmake_minimum_required(VERSION 3.12)
project(entropy_calculator CXX)

set(BOOST_MIN_VERSION "1.61.0")
set(Boost_ADDITIONAL_VERSIONS "1.61.0" "1.61")
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT ${MY_BOOST_DIR})

# Turn on -fPIC -fPIE
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Use Visual Studio folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Require Boost and threade (pthread) support
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
find_package(Threads REQUIRED)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message("UNIX congiguration, Clang")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++")
elseif(${CMAKE_COMPILER_IS_GNUCXX})
    message("UNIX congiguration, GCC")
elseif(${WIN32})
    message("Windows configuraion")
    set(CMAKE_CXX_FLAGS "/EHsc")
    set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

add_subdirectory(entropy)
add_subdirectory(entropy_calculator)


