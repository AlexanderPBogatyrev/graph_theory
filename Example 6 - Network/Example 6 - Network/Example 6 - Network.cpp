#include <iostream>
#include <vector>
#include <tuple>

using namespace std;



struct tup {
    char name;
    string pred_v;
    int time;
};

int ans;

void func(tup T, vector<tup>& arr, vector<int>& way) {
    if (T.pred_v != "-") {
        for (int i = 0; i < T.pred_v.size(); i + 2) func(arr[T.pred_v[i] - 65], arr, way);

        int mx = 1e9;

        for (int i = 0; i < T.pred_v.size(); i + 2) mx = max(mx, way[T.pred_v[i] - 65]);
    }
    else {
        way[T.name - 65] = T.time;
    }
}

int main()
{
    int n, start, end;
    string s, v;

    cin >> n;

    vector<tup> arr;
    vector<int> way(n);

    for (int i = 0; i < n; ++i) {
        tup T;
        cin >> T.name >> T.pred_v >> T.time;
        arr.push_back(T);
    }

    func(arr[arr.size() - 1], arr, way);

    cout << way[arr[arr.size() - 1].name - 65] << endl;

    return 0;
}