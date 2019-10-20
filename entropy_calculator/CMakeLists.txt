set(TARGET entropy_calculator)

find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS program_options REQUIRED)

add_executable(${TARGET})

target_include_directories(${TARGET} 
PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${TARGET} 
PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/command_line_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/random_distributions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET}/command_line_parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET}/random_distributions.h
)

target_link_libraries(${TARGET}
PRIVATE
    ${Boost_LIBRARIES}
    entropy
)

add_dependencies(${TARGET} entropy)
