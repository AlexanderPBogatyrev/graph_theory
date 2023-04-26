#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct edge
{
	int b, w, e;
};

bool comp(const edge& a, const edge& b)
{
	return a.w < b.w;
}

vector <int> p;

int get(int x)
{
	if (x == p[x])
		return x;

	return p[x] = get(p[x]);
}

bool unite(int x, int y)
{
	x = get(x);
	y = get(y);

	if (x == y)
		return false;
	if (x > y) {
		swap(x, y);
	}
	p[x] = y;
	return true;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, ans = 0, x, y;
	cin >> n >> m;

	vector <edge> edges(m);

	p.resize(n);

	for (int i = 0; i < m; i++) {
		cin >> edges[i].b >> edges[i].e >> edges[i].w;
		edges[i].b--;
		edges[i].e--;
	}

	sort(edges.begin(), edges.end(), comp);

	for (int i = 0; i < n; i++) p[i] = i;

	for (int i = 0; i < m; i++) {
		x = edges[i].b;
		y = edges[i].e;
		if (unite(x, y)) ans += edges[i].w;
	}

	cout << ans;
	return 0;
}