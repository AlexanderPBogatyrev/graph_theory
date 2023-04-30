#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

const int INF = -2009000999;

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, t, v;
    string next_v;
    char name;
    cin >> n;

    vector<vector<pair<int, int>>> g(n);
    priority_queue<pair<int, int>> q;
    map<int, int> time;
    vector<int> dist(n, INF);
    vector<bool> next(n);

    for (int i = 0; i < n; ++i) {
        cin >> t;
        time[i] = t;
    }
    for (int i = 0; i < n; ++i) {
        cin >> name >> next_v;
        if (next_v[0] == '-') next[i] = false;
        else next[i] = true;
        for (auto i : next_v) {
            if (i >= 65 && i <= 90) g[i - 'A'].push_back({ name - 'A', time[i - 'A']});
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!next[i]) {
            vector<bool> used(n);
            dist[i] = 0;
            q.push({ 0,  i });
            while (!q.empty()) {
                int a = q.top().second; q.pop();
                if (used[a]) {
                    continue;
                }
                used[a] = true;
                for (auto u : g[a]) {
                    int b = u.first;
                    int w = u.second;
                    if (dist[a] + w > dist[b]) {
                        dist[b] = dist[a] + w;
                        q.push({ -dist[b], b });
                    }
                }
            }
        }
    }
    cout << "Max_Time: " << dist[n - 1] + time[n - 1];

    cout << endl;
    return 0;
}