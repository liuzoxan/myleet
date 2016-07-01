/*
 * leetutils.h
 *
 *  Created on: Jul 11, 2015
 *      Author: zxliu
 */

#ifndef LEETCODE_LEETUTILS_H_
#define LEETCODE_LEETUTILS_H_

#include <iomanip>
#include <iostream>
#include <vector>

namespace leetcode
{

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) :
            val(x), next(nullptr)
    {
    }
};

ListNode * createLinkedList(const std::vector<int> &nums)
{
    ListNode *head = nullptr;
    ListNode *tail = nullptr;
    for (auto iter = nums.begin(); iter != nums.end(); iter++)
    {
        ListNode *node = new ListNode(*iter);
        node->next = nullptr;
        if (head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void destroyLinkedList(ListNode *head)
{
    ListNode *tmp = nullptr;
    while (head)
    {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

void printLinkedList(ListNode *head)
{
    while (head)
    {
        std::cout << std::left << std::setw(4) << head->val;
        head = head->next;
    }
    std::cout << std::endl;
}

}

#endif /* LEETCODE_LEETUTILS_H_ */
