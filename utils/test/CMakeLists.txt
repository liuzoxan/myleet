ADD_EXECUTABLE(terminal_test terminal_test.cpp)
TARGET_LINK_LIBRARIES(terminal_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(terminal_test ${EXECUTABLE_OUTPUT_PATH}/terminal_test)

ADD_EXECUTABLE(log_test log_test.cpp)
TARGET_LINK_LIBRARIES(log_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(log_test ${EXECUTABLE_OUTPUT_PATH}/log_test)

ADD_EXECUTABLE(sleep_test sleep_test.cpp)
TARGET_LINK_LIBRARIES(sleep_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(sleep_test ${EXECUTABLE_OUTPUT_PATH}/sleep_test)

ADD_EXECUTABLE(sync_test sync_test.cpp)
TARGET_LINK_LIBRARIES(sync_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(sync_test ${EXECUTABLE_OUTPUT_PATH}/sync_test)

ADD_EXECUTABLE(timestamp_test timestamp_test.cpp)
TARGET_LINK_LIBRARIES(timestamp_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(timestamp_test ${EXECUTABLE_OUTPUT_PATH}/timestamp_test)

ADD_EXECUTABLE(pidfile_test pidfile_test.cpp)
TARGET_LINK_LIBRARIES(pidfile_test ${UTILS_NEEDED_LIBS} ${CPPUNIT_LIBRARIES})
ADD_TEST(pidfile_test ${EXECUTABLE_OUTPUT_PATH}/pidfile_test)

