/**
 * Дано количество вершин и ребер, исток и сток, затем ребра с их пропускными способностями.
 * Везде 0-индексация. Необходимо вывести максимальный поток и матрицу остаточных пропускных способностей.
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
const int inf = 1e9;

int n, m, s, t;//Количество вершин и ребер, исток и сток
int c[1000][1000];//Матрица пропускных способностей

int main() {
    cin >> n >> m >> s >> t;
    for (int i = 0; i < m; i++) {
        int a, b, cap;
        cin >> a >> b >> cap;
        c[a][b] = cap;
    }

    int MaxFlow = 0;//Искомый максимальный поток
    while (true) {
        /**< Поиск в ширину */
        vector <int> parent(n, -1);
        vector <bool> used(n, false);
        queue <int> q;

        used[s] = true;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int i = 0; i < n; i++) {
                if (!used[i] && c[v][i] > 0) {
                    parent[i] = v;
                    used[i] = true;
                    q.push(i);
                }
            }
        }

        if (!used[t])//Не дошли до стока - поток уже максимальный
            break;

        int AugFlow = inf;//Дополнительный поток

        /**< Бежим по пути и ищем ребро с минимальной пропускной способностью */
        int ptr = t;
        while (ptr != s) {
            AugFlow = min(AugFlow, c[parent[ptr]][ptr]);
            ptr = parent[ptr];
        }

        /**< Изменяем пропускные способности */
        ptr = t;
        while (ptr != s) {
            c[parent[ptr]][ptr] -= AugFlow;//По пути вычитаем
            c[ptr][parent[ptr]] += AugFlow;//Против пути добавляем
            ptr = parent[ptr];
        }

        MaxFlow += AugFlow;
    }

    /**< Выводим ответ */
    printf("%d\n", MaxFlow);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", c[i][j]);
        }
        printf("\n");
    }
    return 0;
}