#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 2009000999;

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m, s, x, u, v, l;
    cin >> n >> m >> s;

    vector<vector<pair<int, int>>> g(n);
    priority_queue<pair<int, int>> q;
    vector<int> dist(n, INF);
    vector<bool> used(n);

    for (int i = 0; i < m; ++i) {
        cin >> u >> v >> l;
        g[u].push_back({ v, l });
        g[v].push_back({ u, l });
    }

    dist[s] = 0;
    q.push({ 0, s });
    while (!q.empty()) {
        int a = q.top().second; q.pop();
        if (used[a]) {
            continue;
        }
        used[a] = true;
        for (auto u : g[a]) {
            int b = u.first;
            int w = u.second;
            if (dist[a] + w < dist[b]) {
                dist[b] = dist[a] + w;
                q.push({ -dist[b], b });
            }
        }
    }

    cout << dist[9];

    cout << endl;
    return 0;
}