#include <bits/stdc++.h>
using namespace std;

int main()
{
	int t;
	// cin >> t;
	t = 13;
	float p;
	float n[13] = {1, 2, 4, 6, 8, 10, 12, 16, 20, 32, 64, 128, 150};
	float s[13] = {
		1, 1.57, 2.46, 2.88, 2.21, 4.63, 3.64, 4.37, 4.02, 4.37, 4.5, 4.13, 4.63

	};
	for (int i = 0; i < t; i++)
	{
		p = (1 - 1 / s[i]) / (1 - 1 / n[i]);
		cout << p * 100 << endl;
	}
	return 0;
}