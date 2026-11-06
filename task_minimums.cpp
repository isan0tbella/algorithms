#include <iostream>
#include <vector>
#include <functional>
#include <queue>


int minimums_amount(const std::vector<std::vector<int>> &g, int v0, int v1){
    std::vector<int> distance(g.size(), -1);
    std::vector<int> ways(g.size(), -1);
    ways[v0] = 1;
    std::queue<int> q;
    distance[v0] = 0;
    q.push(v0);
    
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int u : g[v])
        {
            if (distance[u] == -1)
            {
                distance[u] = distance[v] + 1;
                ways[u] = ways[v];
                q.push(u);
            }
            else if (distance[u] == distance[v] + 1)
            {
                ways[u] += ways[v];
            }
        }
    }


    if (distance[v1] == -1) return 0;
    else return ways[v1];

}