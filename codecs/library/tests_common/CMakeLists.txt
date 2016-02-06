TARGET_NAME()

ADD_LIBRARY("${TARGET_NAME}" STATIC tests_common.h tests_common.cpp)

TARGET_LINK_LIBRARIES("${TARGET_NAME}" external-gtest)