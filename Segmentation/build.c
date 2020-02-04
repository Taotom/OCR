#include "segmentation.h"
#include "../Struct/tree.h"
#include "../Struct/matrix.h"
#include "../Processing/processing.h"
#include <stdlib.h>
#include <stdio.h>

int mystrlen(char *str)
{
    int n = 0;
	while (str[n] != '\0') n++;
	return n;
}

char *mystrcat(char *str, char elm)
{
    int n = mystrlen(str)+2;
	char *concat = malloc(n * sizeof(char));
    for (int i = 0 ; i < n-2 ; i++) concat[i] = str[i];
	concat[n-2] = elm;
	concat[n-1] = '\0';
    return concat;
}


char *write(Tree *T, char *text)	// Go through the tree to write the text
{
	if (T->key >= 0)	// T.key >= 0 mean that the node T represent a char (T.key is an ascii)
	{
		text = mystrcat(text, T->key);
	}

	else if (T->key == -1)	// T.key == -1 mean that the node T represent a word
	{
		Tree *tree = T->child;

		if (tree != NULL)
		{
			text = write(tree, text);

			while (tree->sibling != NULL)
			{
				tree = tree->sibling;
				text = write(tree, text);
			}

			text = mystrcat(text, ' ');
		}
	}

	else if (T->key == -2)	// T.key == -2 mean that the node T represent a line
	{
		Tree *tree = T->child;

		if (tree != NULL)
		{
			text = write(tree, text);

			while (tree->sibling != NULL)
			{
				tree = tree->sibling;
				text = write(tree, text);
			}
			

			text = mystrcat(text, '\n');
		}
	}

	else if (T->key == -3)	// T.key == -1 mean that the node T represent a paragraph
	{
		Tree *tree = T->child;

		if (tree != NULL)
		{
			text = write(tree, text);

			while (tree->sibling != NULL)
			{
				tree = tree->sibling;
				text = write(tree, text);
			}

			text = mystrcat(text, '\n');
			text = mystrcat(text, '\n');
		}
	}

	else if (T->key == -4)	// T.key == -4 mean that the node T is the root of the tree
	{
		Tree *tree = T->child;

		if (tree != NULL)
		{
			text = write(tree, text);

			while (tree->sibling != NULL)
			{
				tree = tree->sibling;
				text = write(tree, text);
			}
		}
	}

	free(T);
	return text;
}

char *build(char *path, char *save, int debug)	// Buid the text extracted from the image segmentation
{
	Matrix img = Img_Matrix(Filtrage(path));

	Tree *Doc = Init_Document(img, debug);

	FILE *file = fopen(save, "w");
	char *str = write(Doc, "");
	fprintf(file, "%s", str);
	fclose(file);
	return str;
}




