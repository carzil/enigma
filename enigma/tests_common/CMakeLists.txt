target_lib(
        SOURCES tests_common.h tests_common.cpp
        LINK_DEPS enigma-common ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES}
)
