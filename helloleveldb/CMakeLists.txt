# apps
ADD_EXECUTABLE(leveldb_example app/example.cpp)
TARGET_LINK_LIBRARIES(leveldb_example ${LEVELDB_LIBRARIES} ${UTILS_NEEDED_LIBS})
