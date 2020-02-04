#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

Tree *new_Tree(int Key)	// Return a new tree
{
	Tree *T = malloc(sizeof(Tree));
	T->key = Key;
	T->sibling = NULL;
	T->child = NULL;

	return T;
}

void AddSibling(Tree *T, Tree *Sibling)	// Add a sibling to a tree
{
	while (T->sibling) T = T->sibling;
	T->sibling = Sibling;
}

void AddChild(Tree *T, Tree *Child)	// Add a child to a tree
{
	if (T->child) AddSibling(T->child, Child);
	else T->child = Child;
}
