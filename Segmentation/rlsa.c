#include "../Struct/matrix.h"

Matrix h_rlsa(Matrix img, int C)	// Executes the RLSA algorithm over the length
{
	Matrix M = copy_Matrix(img);

	for (int j = 0 ; j < M.width ; j++)
	{
		int OneBefore = 0;

		for (int i = 0 ; i < M.height-C ; i++)
		{
			if (M.matrix[i*M.width+j] != 1 && OneBefore == 1)
			{
				int k = 0;

				while (k < C && M.matrix[(i+k)*M.width+j] != 1) k++;

				if (M.matrix[(i+k)*M.width+j] == 1)
				{
					for (int l = 0 ; l < k ; l++)
					{
						M.matrix[(i+l)*M.width+j] = 1;
					}
				}
				else OneBefore = 0;

				i += k;
			}
			else if (M.matrix[i*M.width+j] == 1) OneBefore = 1;
		}

		int lastBP = M.height-C;

		for (int l = M.height-C ; l < M.height ; l++)
		{
			if (M.matrix[l*M.width+j] == 1) lastBP = l;
		}

		for (int m = M.height-C ; m < lastBP ; m++)
		{
			M.matrix[m*M.width+j] = 1;
		}

	}

	return M;
}

Matrix w_rlsa(Matrix img, int C)	// Executes the RLSA algorithm over the width
{
	Matrix M = copy_Matrix(img);

	for (int i = 0 ; i < M.height ; i++)
	{
		int OneBefore = 0;

		for (int j = 0 ; j < M.width-C ; j++)
		{
			if (M.matrix[i*M.width+j] != 1 && OneBefore == 1)
			{
				int k = 0;

				while (k < C && M.matrix[i*M.width+j+k] != 1) k++;

				if (M.matrix[i*M.width+j+k] == 1)
				{
					for (int l = 0 ; l < k ; l++)
					{
						M.matrix[i*M.width+j+l] = 1;
					}
				}
				else OneBefore = 0;

				j += k;
			}
			else if (M.matrix[i*M.width+j] == 1) OneBefore = 1;
		}

		int lastBP = M.width-C;

		for (int l = M.width-C ; l < M.width ; l++)
		{
			if (M.matrix[i*M.width+l] == 1) lastBP = l;
		}

		for (int m = M.width-C ; m < lastBP ; m++)
		{
			M.matrix[i*M.width+m] = 1;
		}
	}

	return M;
}

Matrix rlsa(Matrix img, int wc, int hc)	// Executes the RLSA algorithm
{
	Matrix h_m = h_rlsa(img, hc);
	Matrix w_m = w_rlsa(img, wc);

	Matrix M = copy_Matrix(img);

	for (int i = 0 ; i < M.height ; i++)
	{
		for (int j = 0 ; j < M.width ; j++)
		{
			M.matrix[i*M.width+j] = h_m.matrix[i*h_m.width+j] == 1 || w_m.matrix[i*w_m.width+j] == 1 ? 1 : 0;
		}
	}

	free_Matrix(h_m);
	free_Matrix(w_m);

	return M;
}
