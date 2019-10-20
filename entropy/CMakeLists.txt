set(TARGET entropy)

add_library(${TARGET})

target_include_directories(${TARGET}
PRIVATE
    ${Boost_INCLUDE_DIRS} 
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(${TARGET} 
PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shannon_entropy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET}/shannon_entropy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET}/uint8_codecvt.h
)
