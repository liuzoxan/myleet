#!/usr/bin/env python2
# -*- coding:utf-8 -*-


class BinarySearch(object):
    def __init__(self):
        pass

    @staticmethod
    def search(nums, t):
        length = len(nums)
        low = 0
        high = length - 1
        while low < high:
            mid = (low + high) / 2
            if t < nums[mid]:
                high = mid
            elif t > nums[mid]:
                low = mid
            else:
                return mid
        if t != nums[low]:
            return None
        else:
            return low

def main():
    s = BinarySearch()
    print s.search([1, 2, 3, 4, 6, 9, 10], 9)
    print s.search([-1, 0, 2, 4, 6, 8], 4)
    print s.search([-1, 0, 2, 4, 6, 8], 5)

if __name__ == '__main__':
    main()
