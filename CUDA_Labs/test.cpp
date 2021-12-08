#include <bits/stdc++.h>
using namespace std;

int main()
{
	int i, n = 100000;
	double a[n], b[n], sum = 0;
	for (i = 0; i < n; i++)
	{
		a[i] = b[i] = i;
		sum += a[i] * b[i];
	}
}