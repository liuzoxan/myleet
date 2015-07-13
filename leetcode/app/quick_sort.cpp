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
    static const int CUTOFF = 3;

    static void insertSort(int nums[], int left, int right)
    {
        for (int forward = 1; forward <= right; forward++)
        {
            int saved = nums[forward];
            int backward = forward;
            while(backward > left and nums[backward - 1] > nums[forward])
            {
                nums[backward] = nums[backward - 1];
                backward--;
            }
        }
    }

    static int median(int nums[], int left, int right)
    {
        int med = (left + right) / 2;

        if (nums[left] > nums[med])
            std::swap(nums[left], nums[med]);

        if (nums[left] > nums[right])
            std::swap(nums[left], nums[right]);

        if (nums[med] > nums[right])
            std::swap(nums[med], nums[right]);

        std::swap(nums[med], nums[right - 1]);
        return nums[right - 1];
    }

    static void quickSort(int nums[], int left, int right)
    {
        int forward = left;
        int backward = right - 1;

        if (left + CUTOFF <= right)
        {
            int medNum = median(nums, left, right);

            for ( ; ; )
            {
                while(nums[++forward] < medNum){};
                while(nums[--backward] > medNum){};
                if (forward < backward)
                    std::swap(nums[forward], nums[backward]);
                else
                    break;
            }
            std::swap(nums[forward], nums[right - 1]);
            quickSort(nums, left, forward - 1);
            quickSort(nums, forward + 1, right);
        }
        else
            insertSort(nums, left, right);
    }

};

int main(int argc, char** argv)
{
    int nums[] = {5, 2, 4, 3, 3, 2, 6, 9, 8, 7};

    int size = sizeof(nums)/ sizeof(int);
    Solution::quickSort(nums, 0, size);

    for(int i = 0; i < size; i++)
        cout << nums[i] << ' ';
    cout << endl;
    return EXIT_SUCCESS;
}

