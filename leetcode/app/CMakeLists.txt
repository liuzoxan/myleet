ADD_EXECUTABLE(leet leet.cpp)

ADD_EXECUTABLE(reverse_integer reverse_integer.cpp)

ADD_EXECUTABLE(remove_duplicates_from_sorted_array remove_duplicates_from_sorted_array.cpp)
TARGET_LINK_LIBRARIES(remove_duplicates_from_sorted_array ${LEETCODE_NEEDED_LIBS})

ADD_EXECUTABLE(quick_sort quick_sort.cpp)
TARGET_LINK_LIBRARIES(quick_sort ${LEETCODE_NEEDED_LIBS})
