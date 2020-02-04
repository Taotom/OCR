#include "../Struct/matrix.h"
#include "../Struct/tree.h"
#include "rlsa.h"
#include "../Processing/processing.h"
#include <stdio.h>
#include <stdlib.h>

int Para = 0;
int Line = 0;
int Word = 0;
int Char = 0;

double myfloor(double x)	// Return the integer part of a double
{
	double n = 0;

	while (!((n < x || n == x) & (n+1 > x))) n++;

	return n;
}

Matrix Format(Matrix M)		// Format a matrix in 32x32
{
	Matrix F = new_Matrix(32, 32);

	int min_i = -1;
	int max_i = -1;
	int i = 0, j = 0;

	while ((min_i == -1 || max_i == -1) && i < M.height)	// find the edge of the matrix to resize it
	{
		j = 0;

		while ((min_i == -1 || max_i == -1) && j < M.width)
		{
			if (M.matrix[i*M.width+j] == 1)
			{
				min_i = min_i == -1 ? i : min_i;
			}

			if (M.matrix[(M.height-1-i)*M.width+j] == 1)
			{
				max_i = max_i == -1 ? (M.height-1-i) : max_i;
			}
			j++;
		}

		i++;
	}



	if (min_i != -1)
	{

		M = cut_Matrix(M, min_i, 0, max_i-min_i+1, M.width);	// Resize the matrix to his edge
		int new_height, new_width;
		
		if (M.height > M.width)
		{
			new_height = 32;
			new_width = (int) (M.width * new_height/(double)M.height);
		}
		else
		{
			new_width = 32;
			new_height = (int) (M.height * new_width/(double)M.width);
		}

		Matrix new_M = new_Matrix(new_height, new_width);	// New matrix with the old matrix enlarged or shrunk to fit 32x32
		
		double x, y, x1, y1, x2, y2, x3, y3, a1, a2, a3, a4, v1, v2, v3,v4;

		for (int i = 0 ; i < new_M.height ; i++)	// Bilinear interpolation algorithm
		{
			for (int j = 0 ; j < new_M.width ; j++)
			{

				x = j * M.width/new_M.width;
				y = i * M.height/new_M.height;

				x1 = myfloor(x);
				y1 = myfloor(y);

				x3 = x1+1;
				y3 = y1+1;

				x2 = x1 == M.width-1 ? M.width-1 : x3;
				y2 = y1 == M.height-1 ? M.height-1 : y3;

				a1 = (x-x1)*(y-y1);
				a2 = (x3-x)*(y-y1);
				a3 = (x-x1)*(y3-y);
				a4 = (x3-x)*(y3-y);

				a1 = a1 < 0 ? -a1 : a1;
				a2 = a2 < 0 ? -a2 : a2;
				a3 = a3 < 0 ? -a3 : a3;
				a4 = a4 < 0 ? -a4 : a4;

				v1 = M.matrix[(int) (y1*M.width+x1)] * a4;
				v2 = M.matrix[(int) (y2*M.width+x2)] * a1;
				v3 = M.matrix[(int) (y2*M.width+x1)] * a3;
				v4 = M.matrix[(int) (y1*M.width+x2)] * a2;

				new_M.matrix[i*new_M.width+j] = (v1 + v2 + v3 + v4) > 0.5;
			}
		}

		for (int i = 0 ; i < new_M.height ; i++)	// Fit the matrix to 32x32
		{
			for (int j = 0 ; j < new_M.width ; j++) F.matrix[i*F.width+j] = new_M.matrix[i*new_M.width+j];
		}	

		free_Matrix(new_M);
	}

	return F;
}







void xycut(Matrix img, int detectspace, Tree *T, int debug)	// XYCUT algorithm (cut word and char)
{
	Matrix DetectedBlock;

	int i = img.height/2;
	Matrix original = copy_Matrix(img);
	
	int isblackpixel = 0;
	int y = 0;

	for (int x = 0 ; x < img.width ; x++)
	{
		isblackpixel = 0;
		y = 0;

		while (isblackpixel == 0 && y < img.height)
		{
			isblackpixel = img.matrix[y*img.width+x];
			y++;
		}

		if (isblackpixel == 1)
		{
			for (int z = 0 ; z < img.height ; z++) img.matrix[z*img.width+x] = 1;
		}
	}	


	if (detectspace == 1)	// If cut word
	{
		int sum = 0;
		int cpt = 0;
		int lastp = 0;

		for (int j = 0 ; j < img.width ; j++)
		{
			if (img.matrix[i*img.width+j] == 0)
			{
				sum++;
				lastp = 0;
			}
			else
			{
				if (lastp == 0) cpt++;
				lastp = 1;
			}
		}

		int midspace = sum/(cpt-1);	// Average size of a space between two word

		int dx = 0, dy = 0, fx = 0, fy = img.height-1;
		cpt = 0;
		int debword = 0;
		int endline = 0;
	
		for (int j = 0 ; j < img.width ; j++)
		{
			endline = j == img.width-1 ? 1 : 0;

			if (img.matrix[i*img.width+j] == 1 || endline == 1)
			{
				if (debword == 0 && endline != 1)
				{
					debword = 1;
					dx = j;
					fx = j;
				}
				else
				{
					fx += cpt + 1;

					if ((cpt != 0 && cpt > midspace) || endline == 1)
					{
						fx -= cpt+1;
						DetectedBlock = cut_Matrix(original, dy, dx-1, fy-dy, fx-dx+3);
						
						if (debug == 1)
						{
								char *str = (char*)malloc(10*sizeof(char));
								sprintf(str, "Word/%i", Word);
								Word++;
								Matrix_Img(DetectedBlock, str);
						}

						Tree *Child = new_Tree(-1);
						AddChild(T, Child);


						xycut(DetectedBlock, 0, Child, debug);
						free_Matrix(DetectedBlock);
						debword = 0;
						j = endline == 1 ? j : j-1;	
					}
				}
				
				cpt = 0;
			}
			else
			{
				cpt++;
			}
		}
	}
	else	// If cut char
	{
		int lastp = 0;
		int ascii = 65;
		int dx = 0, dy = 0, fx = 0, fy = img.height-1;
		for (int j = 0 ; j < img.width ; j++)
		{
			if (img.matrix[i*img.width+j] == 0 && lastp == 1)
			{
				DetectedBlock = Format(cut_Matrix(original, dy, dx, fy-dy, fx-dx));
				
				if (debug == 1)
				{
						char *str = (char*)malloc(10*sizeof(char));
						sprintf(str, "Char/%i", Char);
						Char++;
						Matrix_Img(DetectedBlock, str);
				}

				// ascii = Char_Recognition(DetectedBlock);

				Tree *Child = new_Tree(ascii);
				AddChild(T, Child);

				lastp = 0;
			}
			else if (img.matrix[i*img.width+j] == 1)
			{
				if (lastp == 0)
				{
					dx = j;
					fx = j;
				}

				fx++;

				lastp = 1;				
			}
		}
	}

	free_Matrix(original);	
}


void Init_rec(Matrix img, int IsCreateWord, Tree *T, int debug)	// Detection of bloc made by RLSA algorithm (cut paragraph and line)
{
	int D_x = 0, D_y = 0, F_x = 0, F_y = 0;
	int maincpt = 0, maincpt2 = 0;
	int min_j = 0;

	int wc, hc;

	switch (IsCreateWord)
	{
		case 0:		// IsCreateWord ==  mean that the fonction cuts paragraph
			wc = img.width;
			hc = 30;
			break;
		

		default:	// IsCreateWord ==  mean that the fonction cuts line
			wc = img.width;
			hc = 0;
			break;
	}

	Matrix M = rlsa(rlsa(img, wc, hc), wc, hc);

	for (int i = 0 ; i < M.height ; i++)	// Detection of the blocks
	{
		maincpt2 = 0;

		for(int j = min_j ; j < M.width ; j++)
		{
			if (M.matrix[i*M.width+j] == 1)
			{
				maincpt++;
				maincpt2++;

				int b = 1;
				int i2 = i;
				int j2 = j;

				D_y = i;
				D_x = j;
				F_x = j;

				while (i2 < M.height && b ==1)
				{
					int cpt = 0;
					int b2 = 1;

					while (j2 < M.width && b2 == 1)
					{
						if (M.matrix[i2*M.width+j2] == 1)
						{
							if (cpt == 0)
							{
								D_x = j2 < D_x ? j2 : D_x;
								if (j2 > F_x) b = 0;
							}

							cpt++;
						}
						else if (cpt != 0) b2 = 0;

						j2++;
					}

					if (cpt == 0) b = 0;

					if (b == 1) F_x = j2-1 > F_x ? j2-1 : F_x;

					j2 = min_j;
					i2++;
				}

				F_y = i2-2;

				Matrix DetectedBlock = cut_Matrix(img, D_y, D_x, F_y-D_y, F_x-D_x);
				Tree *Child = new_Tree(-3+IsCreateWord);
				AddChild(T, Child);

				if (debug == 1)
				{
						char *str = (char*)malloc(10*sizeof(char));

						switch (IsCreateWord)
						{
						case 0:
								sprintf(str, "Para/%i", Para);
								Para++;
								break;

						default:
								sprintf(str, "Line/%i", Line);
								Line++;
								break;
						}

						Matrix_Img(DetectedBlock, str);
				}

				if (IsCreateWord < 1) Init_rec(DetectedBlock, IsCreateWord+1, Child, debug);
				else if (IsCreateWord == 1) xycut(DetectedBlock, 1, Child, debug);

				free_Matrix(DetectedBlock);
			}

			if (maincpt != 0)
			{
				maincpt = 0;
				i = D_y;
				min_j = F_x;
				j = min_j;
			}
		}
		min_j = 0;
		if (maincpt2 != 0)
		{
			i += F_y-i+1;
		}
	}

	free_Matrix(M);
}

Tree *Init_Document(Matrix img, int debug)
{
	Tree *Document = new_Tree(-4);
	Init_rec(img, 0, Document, debug);
	return Document;
}

