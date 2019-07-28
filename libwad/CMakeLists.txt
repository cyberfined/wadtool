project(libwad)
set(TARGET libwad)
set(SOURCE wad.c lump.c)

add_library(${TARGET} STATIC ${SOURCE})
