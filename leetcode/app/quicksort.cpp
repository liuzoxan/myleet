/*
 * quicksort.cpp
 *
 *  Created on: Jul 12, 2015
 *      Author: zxliu
 */


#include "leetcode/leetcode_fwd.h"

class Solution
{
public:
    void quickSort(vector<int>& nums)
    {
    }

    void insertSort(vector<int>& nums, int start, int end)
    {
        for (int curr = start + 1; curr < end + 1; curr++)
        {
            int saved = nums[curr];
            int back = curr;
            for (; back > start and nums[back - 1] > saved; back--)
                nums[back] = nums[back - 1];
            nums[back] = saved;
        }
    }
};

int main(int argc, char** argv)
{
    int nums[] = {5, 2, 4, 3, 6, 1};
    vector<int> vint(nums, nums + sizeof(nums)/sizeof(int));

    Solution s;
    s.insertSort(vint, 0, 5);

    for (vector<int>::iterator iter = vint.begin(); iter != vint.end(); iter++)
        cout << *iter << " ";

    cout << endl;
    return EXIT_SUCCESS;
}

