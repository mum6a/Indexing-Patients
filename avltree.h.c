//
// Created by Mumba on 29/12/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include "avltree.h.h"
AVLTree CreateTree(void)
{
    AVLTree tree = (AVLTree) malloc(sizeof(struct Node));
    if (tree == NULL)
    {
        printf("Out of memory space!\n");
        return NULL;
    }
    tree->patients = (Patient*) malloc(sizeof(Patient));
    if(tree->patients == NULL)
    {
        printf("Memory allocation failure for creating Patient Node\n");
    }
    tree->left = NULL;
    tree->right = NULL;
    tree->height = 0;
    tree->bmi = 0;
    return tree;
}
int AVLTreeHeight(AVLTree t)
{
    if (t == NULL)
        return -1;
    else
        return t->height;
}

AVLTree SingleRotateWithLeft(AVLTree k1)
{
    AVLTree k2;
    k2 = k1->left;
    k1->left = k2->right;
    k2->right = k1;
    k1->height = Max(AVLTreeHeight(k1->left), AVLTreeHeight(k1->right) + 1);
    k2->height = Max(AVLTreeHeight(k2->left), AVLTreeHeight(k2->right) + 1);
    return k2;
}

AVLTree SingleRotateWithRight(AVLTree k2)
{
    AVLTree k1;
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = Max(AVLTreeHeight(k2->left), AVLTreeHeight(k2->right) + 1);
    k1->height = Max(AVLTreeHeight(k1->left), AVLTreeHeight(k1->right) + 1);
    return k1;
}

AVLTree DoubleRotateWithLeft(AVLTree k3)
{
    k3->left = SingleRotateWithRight(k3->left);
    return SingleRotateWithLeft(k3);
}

AVLTree DoubleRotateWithRight(AVLTree k3)
{
    k3->right = SingleRotateWithLeft(k3->right);
    return SingleRotateWithRight(k3);
}
AVLTree MakeEmptyTree(AVLTree t)
{
    if (t != NULL)
    {
        MakeEmptyTree(t->left);
        MakeEmptyTree(t->right);
        free(t);
    }
    return NULL;
}
int Max(int x, int y)
{
    if (x >= y)
        return x;
    else
        return y;
}