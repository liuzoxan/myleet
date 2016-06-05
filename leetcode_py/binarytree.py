#!/usr/bin/env python2

# -*-coding:utf-8 -*-

class Node(object):
    def __init__(self, value, left, right):
        self.value = value
        self.left = left
        self.right = right

class BinaryTreeOutPut(object):
    def __init__(self):
        pass

    @staticmethod
    def pre_out(tree):
        if tree is None:
            return

        print tree.value
        BinaryTreeOutPut.mid_out(tree.left)
        BinaryTreeOutPut.mid_out(tree.right)

    @staticmethod
    def mid_out(tree):
        if tree is None:
            return

        BinaryTreeOutPut.pre_out(tree.left)
        print tree.value
        BinaryTreeOutPut.pre_out(tree.right)

    @staticmethod
    def post_out(tree):
        if tree is None:
            return

        BinaryTreeOutPut.post_out(tree.left)
        BinaryTreeOutPut.post_out(tree.right)
        print tree.value

    @staticmethod
    def level_out(tree):
        if tree is None:
            return

        stack = []
        stack.append(tree)
        while len(stack) > 0:
            root = stack.pop(0)
            print root.value
            if root.left:
                stack.append(root.left)
            if root.right:
                stack.append(root.right)

    @staticmethod
    def depth(tree):
        if tree is None:
            return 0

        return max(BinaryTreeOutPut.depth(tree.left), BinaryTreeOutPut.depth(tree.right)) + 1

def main():
    tree = Node(3, Node(4, Node(5, None, None), None), Node(6, Node(7, Node(8, None, None), None), Node(9, None, None)))
    BinaryTreeOutPut.mid_out(tree)
    print "====="
    BinaryTreeOutPut.pre_out(tree)
    print "====="
    BinaryTreeOutPut.post_out(tree)
    print "====="
    BinaryTreeOutPut.level_out(tree)
    print "====="
    print BinaryTreeOutPut.depth(tree)

if __name__ == '__main__':
    main()
