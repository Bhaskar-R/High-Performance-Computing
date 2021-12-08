#include <bits/stdc++.h>
using namespace std;
#include <omp.h>
#define n 100
#define m 1000
int main()
{
	srand(time(0));
	double a[n][n], b[n][n], c[n][n] = {{0}};
	float startTime, endTime, execTime;
	int i, k;
	int omp_rank;
	float rtime;
	startTime = omp_get_wtime();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = rand() % 500;
			b[i][j] = rand() % 500;
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	cout << "matrix A" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << a[i][j] << "\t";
		cout << endl;
	}
	cout << "matrix B" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << b[i][j] << "\t";
		cout << endl;
	}
	cout << "resultant matrix " << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << c[i][j] << "\t";
		cout << endl;
	}
	endTime = omp_get_wtime();
	execTime = endTime - startTime;
	rtime = execTime;
	printf("\n rtime=%f\n", rtime);
	return (0);
}
