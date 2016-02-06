TARGET_NAME()

ADD_LIBRARY(${TARGET_NAME} STATIC
    codec.h codec.cpp
    sample.h sample.cpp
)
