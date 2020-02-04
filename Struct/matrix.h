#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix
{
	int width;
	int height;
	double *matrix;
} Matrix;

double maxE_Matrix(Matrix M);
Matrix new_Matrix(int H, int W);
Matrix new_alea_Matrix(int H, int W, double min, double max);
Matrix dot_Matrix(Matrix M, Matrix P);
Matrix dotBE_Matrix(Matrix M, double e);
Matrix dotE_Matrix(Matrix M, Matrix P);
Matrix copy_Matrix(Matrix M);
Matrix cut_Matrix(Matrix M, int x, int y, int height, int width);
Matrix addT_Matrix(Matrix M, Matrix N);
Matrix addE_Matrix(Matrix M, Matrix N);
Matrix add_Matrix(Matrix M, double value);
void print_Matrix(Matrix M);
void free_Matrix(Matrix M);


#endif
