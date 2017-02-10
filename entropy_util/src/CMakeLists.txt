set(TARGET entropy_util)

find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS program_options REQUIRED)

file(GLOB SOURCES *.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../include/${TARGET}/*.h)

include_directories(${Boost_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/entropy/include 
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
    ${Boost_LIBRARIES}
    entropy)

add_dependencies(${TARGET} entropy)

