AUX_SOURCE_DIRECTORY(src/ LEETCODELIB_FILES)
ADD_LIBRARY(leetcodelib SHARED ${LEETCODELIB_FILES})
SET_TARGET_PROPERTIES(
        leetcodelib
    PROPERTIES
        SOVERSION ${${PROJECT_NAME}_SOVERSION}
        VERSION ${${PROJECT_NAME}_VERSION})
INSTALL(TARGETS leetcodelib LIBRARY DESTINATION lib${LIB_SUFFIX})

ADD_EXECUTABLE(leet app/leet.cpp)

ADD_EXECUTABLE(reverse_integer app/reverse_integer.cpp)

ADD_EXECUTABLE(remove_duplicates_from_sorted_array app/remove_duplicates_from_sorted_array.cpp)
TARGET_LINK_LIBRARIES(remove_duplicates_from_sorted_array ${LEETCODE_NEEDED_LIBS})

ADD_EXECUTABLE(quick_sort app/quick_sort.cpp)
TARGET_LINK_LIBRARIES(quick_sort ${LEETCODE_NEEDED_LIBS})
