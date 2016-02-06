TARGET_NAME()

ADD_LIBRARY("${TARGET_NAME}" STATIC zlib.h zlib.cpp)
TARGET_LINK_LIBRARIES("${TARGET_NAME}" library-common external-zlib)

ADD_SUBDIRECTORY(test)