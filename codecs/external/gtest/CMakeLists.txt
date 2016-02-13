set(gtest_build_tests OFF)
add_subdirectory(gtest-1_7_0)

TARGET_NAME()

TARGET_LIB(NAME "${TARGET_NAME}"
           SOURCES gtest.h gtest.cpp
           LINK_DEPS gtest
           )

target_include_directories("${TARGET_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/gtest-1_7_0/include")

TARGET_LIB(NAME "${TARGET_NAME}_main"
           SOURCES gtest.h gtest.cpp
           LINK_DEPS gtest_main
           )

target_include_directories("${TARGET_NAME}_main" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/gtest-1_7_0/include")
