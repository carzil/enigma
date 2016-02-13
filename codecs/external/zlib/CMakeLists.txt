add_subdirectory(zlib-1_2_8)

TARGET_LIB(
        SOURCES zlib.h zlib.cpp
        LINK_DEPS zlibstatic
)

add_subdirectory(test)
