set(COMMON_LIB "lib_common")

file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${COMMON_LIB} STATIC ${COMMON_SOURCES})

target_include_directories(${COMMON_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${COMMON_LIB} PRIVATE lib_mandelbrot_wrapper)
