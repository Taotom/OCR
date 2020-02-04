#ifndef TREE_H
#define TREE_H

typedef struct Tree
{
	int key; // key >= 0 -> ascii ; key = -1 -> word ; key = -2 -> line ; key = -3 -> para ; key = -4 -> doc
	struct Tree *child;
	struct Tree *sibling;
} Tree;


Tree *new_Tree(int Key);
void AddChild(Tree *T, Tree *Child);
void AddSibling(Tree *T, Tree *Sibling);
void print_Tree(Tree *T);

#endif
