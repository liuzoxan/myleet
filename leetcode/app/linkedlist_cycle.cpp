/*
 * linkedlistcycle.cpp
 *
 *  Created on: Jul 1, 2016
 *      Author: zxliu
 */

/**
 * LeetCode, Linked List Cycle
 * time O(n), space O(1)
 *
 * Given a linked list, determine if it has a cycle in it.
 * Follow up: Can you solve it without using extra space?
 */

#include "leetcode/leetcode_fwd.h"

class Solution
{
public:
    bool hasCycle(ListNode *head)
    {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast)
                return true;
        }
        return false;
    }
};

int main(int argc, char **argv)
{
    vector<int> nums
    { 1, 2, 3, 4, 5 };
    ListNode *head = createLinkedList(nums);
    printLinkedList(head);
    destroyLinkedList(head);

    Solution sl;
    if (sl.hasCycle(head))
    {
        Log::info("Has cycle");
    }
    else
    {
        Log::error("No cycle");
    }
    return EXIT_SUCCESS;
}
