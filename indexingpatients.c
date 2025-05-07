#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h.h"
#define FALSE 0
#define TRUE 1
AVLTree insertPatient(AVLTree, char *, char *, char *, float, float, float);
void youngestPatient(AVLTree, Patient **);
AVLTree loadPatients(char *filename);
void parseDate(const char *, int *, int *, int *);
int isYounger(const char *, const char *);
void findYoungest(AVLTree);
void infoPatient(AVLTree, float);
void displayPatients(AVLTree);
int main(int argc, char *argv[])
{
    int exit;
    int command;
    exit = FALSE;
    AVLTree tree = loadPatients(argv[1]);
    while (!exit)
    {
        printf("Welcome to patient Indexing!!!"
               "\n------Menu------\n"
               "1)Display the full index "
               "\n2)Display the details of the patients"
               "\n3)Display the youngest patient"
               " \n4)Exit "
               "\nEnter your option: ");
        scanf("%d", &command);
        switch (command)
       {
       case 1:
           displayPatients(tree);
          break;
       case 2:
          printf("Enter BMI: ");
          float bmi;
          scanf("%f", &bmi);
          infoPatient(tree,bmi);
          break;
       case 3:
           findYoungest(tree);
          break;
       case 4:
          exit = TRUE;
          break;
       default:
          printf("command not recognized\n");
          break;
       }
    }
    printf("\n");
    system("PAUSE");
    return 0;
}
AVLTree loadPatients(char *filename) {
    char name[20], surname[20], dob[25];
    float height, weight, bmi;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return NULL;
    }
    AVLTree tree = CreateTree();
    while (fscanf(file, "%[^;];%[^;];%[^;];%f;%f;%f\n", name, surname, dob, &height, &weight, &bmi) == 6)
    {
        tree = insertPatient(tree, name, surname, dob, height, weight, bmi);
    }
    fclose(file);
    return tree;
}
AVLTree insertPatient(AVLTree tree, char *name, char *surname, char *dob, float height, float weight, float bmi)
{
    if (tree == NULL) {
        tree = malloc(sizeof(struct Node));
        if (!tree) {
            printf("Out of memory!\n");
            return NULL;
        }
        tree->bmi = bmi;
        tree->patients = malloc(sizeof(Patient));
        strcpy(tree->patients->name, name);
        strcpy(tree->patients->surname, surname);
        strcpy(tree->patients->dob, dob);
        tree->patients->height = height;
        tree->patients->weight = weight;
        tree->patients->next = NULL;
        tree->left = tree->right = NULL;
        tree->height = 0;
    }
    else if (tree->bmi == 0) {
        tree->bmi = bmi;
        strcpy(tree->patients->name, name);
        strcpy(tree->patients->surname, surname);
        strcpy(tree->patients->dob, dob);
        tree->patients->height = height;
        tree->patients->weight = weight;
        tree->patients->next = NULL;
    }
    else if (bmi < tree->bmi) {
        tree->left = insertPatient(tree->left, name, surname, dob, height, weight, bmi);
        if (AVLTreeHeight(tree->left) - AVLTreeHeight(tree->right) == 2) {
            if (bmi < tree->left->bmi)
                tree = SingleRotateWithLeft(tree);
            else
                tree = DoubleRotateWithLeft(tree);
        }
    } else if (bmi > tree->bmi) {
        tree->right = insertPatient(tree->right, name, surname, dob, height, weight, bmi);
        if (AVLTreeHeight(tree->right) - AVLTreeHeight(tree->left) == 2) {
            if (bmi > tree->right->bmi)
                tree = SingleRotateWithRight(tree);
            else
                tree = DoubleRotateWithRight(tree);
        }
    } else {
        // Add the patient to the existing BMI node
        Patient *newPatient = malloc(sizeof(Patient));
        strcpy(newPatient->name, name);
        strcpy(newPatient->surname, surname);
        strcpy(newPatient->dob, dob);
        newPatient->height = height;
        newPatient->weight = weight;
        newPatient->next = tree->patients;
        tree->patients = newPatient;
    }
    tree->height = Max(AVLTreeHeight(tree->left), AVLTreeHeight(tree->right)) + 1;
    return tree;
}
void displayPatients(AVLTree tree)
{
    if (tree != NULL) {
        displayPatients(tree->left);
        Patient *current = tree->patients;
        while (current) {
            printf("%s;%s;%s;%.2f;%.2f;%.2f\n", current->name, current->surname, current->dob, current->height, current->weight, tree->bmi);
            current = current->next;
        }
        displayPatients(tree->right);
    }
}
void youngestPatient(AVLTree tree, Patient **youngest) {
    if (tree == NULL) return;

    Patient *current = tree->patients;
    while (current) {
        if (*youngest == NULL || isYounger(current->dob, (*youngest)->dob)) {
            *youngest = current;
        }
        current = current->next;
    }

    youngestPatient(tree->left, youngest);
    youngestPatient(tree->right, youngest);
}
void findYoungest(AVLTree tree) {
    Patient *youngest = NULL;
    youngestPatient(tree, &youngest);
    if (youngest)
    {
        printf("%s %s is the youngest patient.\n",
               youngest->name, youngest->surname);
    } else
    {
        printf("No patients found.\n");
    }
    /*
Complexity Analysis:
1. Time Complexity:
   - Traverses all nodes in the AVL tree to find the youngest patient. This takes O(n), where n is the number of nodes in the tree.
   - Checking patients within a node involves iterating through the linked list, which adds O(k) for k patients in each node.
   - Total complexity: O(n + K), where K is the total number of patients across all nodes.
Improvement:
1. Augment the AVL tree nodes to store a pointer to the youngest patient in the subtree rooted at that node.
   - This allows finding the youngest patient in O(log n) time.
   - During insertions or deletions, update the "youngest patient" field in O(log n) time as you traverse the tree.
2. Use a specialized data structure (e.g., a priority queue) to maintain all patients sorted by date of birth.
   - This would allow finding the youngest patient in O(1) time but may increase insertion complexity to O(log n).
*/
}
void parseDate(const char *dob, int *day, int *month, int *year) {
    sscanf(dob, "%d/%d/%d", day, month, year);
}
int isYounger(const char *dob1, const char *dob2)
{
    int day1, month1, year1, day2, month2, year2;
    parseDate(dob1, &day1, &month1, &year1);
    parseDate(dob2, &day2, &month2, &year2);
    if (year1 > year2) return 1;
    if (year1 < year2) return 0;
    if (month1 > month2) return 1;
    if (month1 < month2) return 0;
    return day1 > day2;
}
void infoPatient(AVLTree tree, float bmi) {
    if (tree == NULL)
    {
        printf("There is not an available patient who has BMI of %.1f\n", bmi);
        return;
    }
    else
    {
        if (bmi < tree->bmi)
        {
            infoPatient(tree->left, bmi);
        } else if (bmi > tree->bmi)
        {
            infoPatient(tree->right, bmi);
        } else
        {
            Patient *current = tree->patients;
            while (current)
            {
                printf("%s;%s;%s;%.2f;%.2f;%.2f\n", current->name, current->surname, current->dob, current->height,
                       current->weight, bmi);
                current = current->next;
            }
        }
    }
    /*
Complexity Analysis:
1. Time Complexity:
   - The search for the BMI node in the AVL tree takes O(log n), where n is the number of nodes in the tree.
   - Displaying patients in the linked list for the matching BMI takes O(k), where k is the number of patients with the same BMI.
   - Total complexity: O(log n + k).
     Improvement:
1. Instead of using a linked list to store patients with the same BMI, a dynamic array could be used.
   - This would allow binary search on patients if additional sorting (e.g., by name) is required.
   - Insertion into the array could be handled by dynamically resizing it when needed.
*/
}
