set(COMMON_LIB "lib_common")

file(GLOB C_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB C_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/common/*.h)

add_library(${COMMON_LIB} STATIC ${C_SOURCES} ${C_HEADERS})

target_include_directories(${COMMON_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${COMMON_LIB} PUBLIC lib_mandelbrot_wrapper)
