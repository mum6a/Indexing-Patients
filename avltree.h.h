//
// Created by Mumba on 29/12/2024.
//

typedef struct Patient {
    char name[50];
    char surname[50];
    char dob[15];
    float height;
    float weight;
    struct Patient *next;
} Patient;
typedef struct Node *AVLTree;
struct Node {
    float bmi;
    Patient *patients; // List of patients with the same BMI
    AVLTree left;
    AVLTree right;
    int height;
};
#ifndef CNG_213_ASSIGNMENT_3_AVLTREE_H_H
#define CNG_213_ASSIGNMENT_3_AVLTREE_H_H
AVLTree CreateTree(void);
AVLTree MakeEmptyTree(AVLTree);
AVLTree SingleRotateWithLeft(AVLTree);
AVLTree SingleRotateWithRight(AVLTree);
AVLTree DoubleRotateWithLeft(AVLTree);
AVLTree DoubleRotateWithRight(AVLTree);
int AVLTreeHeight(AVLTree);
void DisplayTreeStructure(AVLTree, int);
int Max(int, int);
#endif //CNG_213_ASSIGNMENT_3_AVLTREE_H_H
