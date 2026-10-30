#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

std::vector<int> topo_sort(const std::vector<std::vector<int>> &g)
{
    std::vector<bool> visited(g.size());
    std::vector<int> result;

    std::function<void(int)> dfs = [&](int v)
    {
        visited[v] = true;
        for (int u : g[v])
            if (!visited[u])
                dfs(u);
        result.push_back(v);
    };

    for (int v = 0; v < g.size(); ++v)
        if (!visited[v])
            dfs(v);

    std::reverse(result.begin(), result.end());
    return result;
}

// сама задача:

int min_v(const std::vector<std::vector<int>> &g)
{
    int n = g.size();

    // косарайю, считаю компоненты
    std::vector<std::vector<int>> reverse_g(n);
    for (int v = 0; v < n; ++v)
        for (int u : g[v])
            reverse_g[u].push_back(v);

    std::vector<int> order = topo_sort(g);

    std::vector<int> component(n, -1);
    int k = 0;

    // по обратному, добавляя компоненты
    std::function<void(int)> dfs = [&](int v)
    {
        component[v] = k;
        for (int u : reverse_g[v])
            if (component[u] == -1)
                dfs(u);
    };

    for (int v : order)
    {
        if (component[v] == -1)
        {
            dfs(v);
            ++k;
        }
    }

    if (k == 1)
        return 0;

    // для каждого входящие/исходящие
    std::vector<bool> has_in(k, false);
    std::vector<bool> has_out(k, false);

    for (int v = 0; v < n; ++v)
        for (int u : g[v])
            if (component[v] != component[u])
            {
                has_out[component[v]] = true;
                has_in[component[u]] = true;
            }

    // считаю компоненты без входящих/исходящих. их максимум - столько же нужно добавить рёбер
    int ins = 0;  // нет входящих
    int outs = 0; // нет исходящих

    for (int i = 0; i < k; ++i)
    {
        if (!has_in[i])
            ++ins;
        if (!has_out[i])
            ++outs;
    }

    return std::max(ins, outs);
}

int main()
{
    std::vector<std::vector<int>> g = {
        {1}, // из 0 есть ребро в 1
        {2}, // из 1 есть ребро в 2
        {}   // из 2 рёбер нет
    };

    std::cout << min_v(g) << '\n'; //1
}