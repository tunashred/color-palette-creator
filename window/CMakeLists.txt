set(WINDOW_MANAGER_LIB "lib_window_manager")

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${WINDOW_MANAGER_LIB} STATIC ${C_SOURCES})

target_include_directories(${WINDOW_MANAGER_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${WINDOW_MANAGER_LIB} PRIVATE lib_palette_creator lib_common lib_mandelbrot_wrapper)
