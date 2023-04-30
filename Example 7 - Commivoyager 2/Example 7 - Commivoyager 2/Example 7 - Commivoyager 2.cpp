#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <cstring>
#include <climits>
using namespace std;

// `N` - общее количество узлов на Graphе или городов на карте
#define N 4

// Сторожевое значение для представления `INFINITY`
#define INF INT_MAX

// Узлы State Space Tree
struct Node
{
    // сохраняет ребра дерева в пространстве состояний
    // помощь в отслеживании пути при нахождении ответа
    vector<pair<int, int>> path;

    // сохраняет уменьшенную матрицу
    int reducedMatrix[N][N];

    // сохраняет нижнюю границу
    int cost;

    // сохраняет текущий номер города
    int vertex;

    // сохраняет общее количество посещенных городов
    int level;
};

// Функция выделения нового узла `(i, j)` соответствует посещению города `j`
// из города `i`
Node* newNode(int parentMatrix[N][N], vector<pair<int, int>> const& path,
    int level, int i, int j)
{
    Node* node = new Node;

    // сохраняет ребра-предки дерева в пространстве состояний
    node->path = path;
    // пропустить корневой узел
    if (level != 0)
    {
        // добавляем к пути текущее ребро
        node->path.push_back(make_pair(i, j));
    }

    // копируем данные из родительского узла в текущий узел
    memcpy(node->reducedMatrix, parentMatrix,
        sizeof node->reducedMatrix);

    // Изменяем все записи строки `i` и столбца `j` на `INFINITY`
    // пропустить корневой узел
    for (int k = 0; level != 0 && k < N; k++)
    {
        // устанавливаем исходящие ребра для города `i` в `INFINITY`
        node->reducedMatrix[i][k] = INF;

        // устанавливаем входящие ребра в город `j` на `INFINITY`
        node->reducedMatrix[k][j] = INF;
    }

    // Установите `(j, 0)` в `INFINITY`
    // здесь начальный узел равен 0
    node->reducedMatrix[j][0] = INF;

    // установить количество посещенных городов на данный момент
    node->level = level;

    // присваиваем номер текущего города
    node->vertex = j;

    // return node
    return node;
}

// Функция для сокращения каждой строки так, чтобы в каждой строке был хотя бы один ноль
void rowReduction(int reducedMatrix[N][N], int row[N])
{
    // инициализируем массив строк значением `INFINITY`
    fill_n(row, N, INF);

    // `row[i]` содержит минимум в строке `i`
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (reducedMatrix[i][j] < row[i]) {
                row[i] = reducedMatrix[i][j];
            }
        }
    }

    // уменьшаем минимальное значение каждого элемента в каждой строке
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (reducedMatrix[i][j] != INF && row[i] != INF) {
                reducedMatrix[i][j] -= row[i];
            }
        }
    }
}

// Функция для уменьшения каждого столбца так, чтобы там был хотя бы один ноль
// в каждом столбце
void columnReduction(int reducedMatrix[N][N], int col[N])
{
    // инициализируем все элементы массива `col` значением `INFINITY`
    fill_n(col, N, INF);

    // `col[j]` содержит минимум в col `j`
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (reducedMatrix[i][j] < col[j]) {
                col[j] = reducedMatrix[i][j];
            }
        }
    }

    // уменьшаем минимальное значение каждого элемента в каждом столбце
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (reducedMatrix[i][j] != INF && col[j] != INF) {
                reducedMatrix[i][j] -= col[j];
            }
        }
    }
}

// Функция для получения нижней границы пути, начинающегося с текущего минимального узла
int calculateCost(int reducedMatrix[N][N])
{
    // инициализировать стоимость до 0
    int cost = 0;

    // Сокращение строк
    int row[N];
    rowReduction(reducedMatrix, row);

    // Сокращение столбца
    int col[N];
    columnReduction(reducedMatrix, col);

    // общая ожидаемая стоимость равна сумме всех сокращений
    for (int i = 0; i < N; i++)
    {
        cost += (row[i] != INT_MAX) ? row[i] : 0,
            cost += (col[i] != INT_MAX) ? col[i] : 0;
    }

    return cost;
}

// Функция для печати списка посещенных городов по наименьшей стоимости
void printPath(vector<pair<int, int>> const& list)
{
    for (int i = 0; i < list.size(); i++) {
        cout << list[i].first + 1 << " -> " << list[i].second + 1 << endl;
    }
}

// Объект сравнения, который будет использоваться для упорядочивания кучи
struct comp
{
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->cost > rhs->cost;
    }
};

// Функция для решения задачи о коммивояжёре с использованием метода ветвей и границ
int solve(int costMatrix[N][N])
{
    // Создаем приоритетную очередь для хранения активных узлов дерева поиска
    priority_queue<Node*, vector<Node*>, comp> pq;

    vector<pair<int, int>> v;

    // создаем корневой узел и вычисляем его стоимость.
    // TSP начинается с первого города, т.е. узла 0
    Node* root = newNode(costMatrix, v, 0, -1, 0);

    // получаем нижнюю границу пути, начинающегося в узле 0
    root->cost = calculateCost(root->reducedMatrix);

    // Добавляем корень в список живых узлов
    pq.push(root);

    // Находит живой узел с наименьшей стоимостью, добавляет его потомков в список
    // живые узлы и, наконец, удаляем их из списка
    while (!pq.empty())
    {
        // Находим активный узел с наименьшей оценочной стоимостью
        Node* min = pq.top();

        // Найденный узел удаляется из списка живых узлов
        pq.pop();

        // `i` хранит текущий номер города
        int i = min->vertex;

        // если все города посещены
        if (min->level == N - 1)
        {
            // вернуться в начальный город
            min->path.push_back(make_pair(i, 0));

            // вывести список посещенных городов
            printPath(min->path);

            // возвращаем оптимальную стоимость
            return min->cost;
        }

        // делаем для каждого дочернего элемента min
        // `(i, j)` образует ребро в пространственном дереве
        for (int j = 0; j < N; j++)
        {
            if (min->reducedMatrix[i][j] != INF)
            {
                // создаем дочерний узел и вычисляем его стоимость
                Node* child = newNode(min->reducedMatrix, min->path,
                    min->level + 1, i, j);

                /* Стоимость дочернего элемента =
                    стоимость родительской ноды +
                    стоимость ребра(i, j) +
                    нижняя граница пути, начинающегося в узле j
                */

                child->cost = min->cost + min->reducedMatrix[i][j]
                    + calculateCost(child->reducedMatrix);

                // Добавляем дочерний элемент в список активных узлов
                pq.push(child);
            }
        }

        // свободный узел, так как мы уже сохранили ребра `(i, j)` в векторе.
        // Так что нет необходимости в родительском узле при печати решения.
        delete min;
    }
}

int main()
{
    // матрица затрат для задачи коммивояжера.

    // стоимость 34


    int costMatrix[N][N];
    /*{
        { INF, 5, 11, 9},
        { 9, INF, 8, 7},
        { 11, 10, INF, 6},
        { 12, 6, 15, INF}
    };*/

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            else cin >> costMatrix[i][j];
        }
    }

    cout << "\n\nTotal cost is " << solve(costMatrix);

    return 0;
}
