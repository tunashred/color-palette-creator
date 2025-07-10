set(PALETTE_CREATOR_LIB "lib_palette_creator")

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${PALETTE_CREATOR_LIB} STATIC ${C_SOURCES})

target_include_directories(${PALETTE_CREATOR_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PALETTE_CREATOR_LIB} PRIVATE lib_common)
