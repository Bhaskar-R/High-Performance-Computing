#include <bits/stdc++.h>
using namespace std;

int main()
{
	int t;
	cin >> t;
	while (t--)
	{
		float s, n, p;
		cin >> s >> n;
		p = (1 - 1 / s) / (1 - 1 / n);
		cout << p << endl;
	}
	return 0;
}