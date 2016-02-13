TARGET_LIB(
    SOURCES zlib.h zlib.cpp
    LINK_DEPS external-zlib
)

add_subdirectory(test)
