set(PALETTE_CREATOR_LIB "lib_palette_creator")

file(GLOB C_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB C_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/palette_creator/*.h)

add_library(${PALETTE_CREATOR_LIB} STATIC ${C_SOURCES} ${C_HEADERS})

target_include_directories(${PALETTE_CREATOR_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PALETTE_CREATOR_LIB} PUBLIC lib_common -lm)
