# apps or test

ADD_EXECUTABLE(valist valist.cpp)

ADD_EXECUTABLE(logtest logtest.cpp)
TARGET_LINK_LIBRARIES(logtest ${UTILS_NEEDED_LIBS})
