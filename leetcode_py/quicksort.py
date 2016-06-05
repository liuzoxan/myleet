#!/usr/bin/env python2
# -*- coding:utf-8 -*


class QuickSort(object):
    def __init__(self):
        pass

    def sort(self, nums):
        if nums == []:
            return nums

        pivot = nums[0]
        lesser = self.sort([x for x in nums[1:] if x < pivot])
        greater = self.sort([x for x in nums[1:] if x >= pivot])
        return lesser + [pivot] + greater


def main():
    s = QuickSort()
    print s.sort([2, 3, 1, 8, 6, 6])
    print s.sort([-6, 9, 3, 1, -2, 6, 6, -2, 9, 8])

if __name__ == '__main__':
    main()
