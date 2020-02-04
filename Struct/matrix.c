#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>



double randfrom(double min, double max)	// Return a randmon double between min and max
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}




Matrix new_Matrix(int H, int W)	// Create a new matrix init with 0
{
        Matrix M;

        M.height = H;
        M.width = W;

	M.matrix = malloc(H*W*sizeof(double));
	for (int i = 0 ; i < H*W ; i++)
	{
		M.matrix[i] = 0.0;
	}

	return M;
}

Matrix new_alea_Matrix(int H, int W, double min, double max)	// Create a new matrix init with random double
{
	Matrix M;

        M.height = H;
        M.width = W;

        M.matrix = malloc(H*W*sizeof(double));
        for (int i = 0 ; i < H*W ; i++)
        {
                M.matrix[i] = randfrom(min, max);
        }

        return M;

}


Matrix dot_Matrix(Matrix M, Matrix P)	// Return the product between M and P
{
	if (M.width != P.height) err(1, "M.width != P.height");

	Matrix R;
	R.height = M.height;
	R.width = P.width;
	R.matrix = malloc(R.height*R.width*sizeof(double));

	for (int i = 0 ; i < R.height ; i++)
	{
		for (int j = 0 ; j < R.width ; j++)
		{
			for (int k = 0 ; k < M.width ; k++)
			{
				R.matrix[i*R.width+j] += M.matrix[i*M.width+k]*P.matrix[k*P.width+j];
			}
		}
	}

	return R;
}


Matrix dotE_Matrix(Matrix M, Matrix P)	// Return the product element by element between M and P
{

	if (M.width != P.width || M.height != P.height) err(1,"M.width != P.width || M.height != P.height");

	for (int i = 0 ; i < M.width*M.height ; i++) M.matrix[i] *= P.matrix[i];

	return M;
}

Matrix dotBE_Matrix(Matrix M, double e)	// Return the product of M by a double
{
	for (int i = 0 ; i < M.width*M.height ; i++) M.matrix[i] *= e;
	return M;
}

Matrix add_Matrix(Matrix M, double value)	// Return the matrix with the added value
{
	for (int i = 0 ; i < M.height ; i++)
	{
		for (int j = 0 ; j < M.width ; j++)
		{
			M.matrix[i*M.width+j] += value;
		}
	}

	return M;
}

void print_Matrix(Matrix M)	// Print a matrix
{
	for (int i = 0 ; i < M.height ; i++)
        {
                for (int j = 0 ; j < M.width ; j++)
                        printf("%.8f    ", M.matrix[i*M.width+j]);
		printf("\n");
        }

	printf("\n");
}

void free_Matrix(Matrix M)	// Free space of a matrix
{
	free(M.matrix);
	M.width = 0;
	M.height = 0;
}


Matrix copy_Matrix(Matrix M)	// Copy of a matrix
{
	Matrix copy;
	copy.height = M.height;
	copy.width = M.width;
	copy.matrix = malloc(copy.height*copy.width*sizeof(double));

	for (int i = 0 ; i < copy.height*copy.width ; i++) copy.matrix[i] = M.matrix[i];

	return copy;
}


Matrix cut_Matrix(Matrix M, int x, int y, int height, int width)	// Return the cuted matrix (part of the original matrix)
{
        Matrix cut;
        cut.height = height;
        cut.width = width;
        cut.matrix = malloc(height*width*sizeof(double));

        for (int i = x ; i < x+cut.height ; i++)
	{
		for (int j = y ; j < y+cut.width ; j++) cut.matrix[(i-x)*cut.width+(j-y)] = M.matrix[i*M.width+j];
	}

        return cut;
}

double maxE_Matrix(Matrix M)	// Return the max element of a matrix
{
	double max = M.matrix[0];
	for (int i = 0 ; i < M.width*M.height ; i++) max = (max < M.matrix[i]) ? M.matrix[i] : max;
	return max;
}

Matrix addT_Matrix(Matrix M, Matrix N)	// Return the addition of M and N
{
	if (M.height != N.height) err(1, "M.height != N.height");

	for (int i = 0 ; i < M.height ; i++)
	{
		for (int j = 0 ; j < M.width ; j++)
		{
			M.matrix[i*M.width+j] += N.matrix[i];
		}
	}

	return M;
}

Matrix addE_Matrix(Matrix M, Matrix N) // Return the addition of M and N
{
	if (M.height != N.height || M.width != N.width) err(1, "M.height != N.height || M.width != N.width");

	for (int i = 0 ; i < M.height ; i++)
	{
		for (int j = 0 ; j < M.width ; j++)
		{
			M.matrix[i*M.width+j] += N.matrix[i*N.width+j];
		}
	}

	return M;
}
