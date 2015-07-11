/*
 * remove_duplicates_from_sorted_array.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: zxliu
 */

/*
 *  Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.

Do not allocate extra space for another array, you must do this in place with constant memory.

For example,
Given input array nums = [1,1,2],

Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively. It doesn't matter what you leave beyond the new length.
*/

#include "leetcode/leetcode_fwd.h"

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int currIdx = 0;
        int nextIdx = currIdx + 1;

        while (nextIdx < nums.size()) {
            if (nums[currIdx] != nums[nextIdx]) {
                nums[currIdx + 1] = nums[nextIdx];
                currIdx++;
            }

            nextIdx++;
        }

        return ++currIdx;
    }

};

int main(int argc, char** argv) {

    int nums[] = {1, 1, 2, 3, 3, 3, 4, 5, 5};
    int size = sizeof(nums)/sizeof(int);
    vector<int> vint(nums, nums + size);
    Solution s;
    int length = s.removeDuplicates(vint);
    Log::debug("Length before %d, after %d", size, length);

    for (vector<int>::iterator iter = vint.begin(); iter != vint.end(); iter++)
        cout << *iter << " ";

    cout << endl;

    return EXIT_SUCCESS;
}
