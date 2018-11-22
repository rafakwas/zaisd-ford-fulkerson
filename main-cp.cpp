using namespace std;

#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
#include<iostream>
#include<list>

#define V 6

bool dfs(int rGraph[V][V], int x, int t, int * parent, bool * visited){
    if(x == t) return true;
    visited[x] = true;
    for(int i = 0; i < V; ++i){
        if(rGraph[x][i] > 0 && !visited[i]){
            parent[i] = x;
            if(dfs(rGraph, i, t, parent, visited))
                return true;
        }
    }
    return false;
}

bool dfs(int rGraph[V][V], int s, int t, int parent[])
{
    parent[s] = -1;
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    return dfs(rGraph, s, t, parent, visited);
}

int fordFulkerson(int graph[V][V], int s, int t)
{
    int rGraph[V][V];
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];
    int parent[V];
    int max_flow = 0;
    while (dfs(rGraph, s, t, parent))
    {
        int path_flow = INT_MAX;
        for (int v=t; v!=s; v=parent[v])
        {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (int v=t; v != s; v=parent[v])
        {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

int main()
{
    int graph[V][V] = { {0, 16, 13, 0, 0, 0},
                        {0, 0, 10, 12, 0, 0},
                        {0, 4, 0, 0, 14, 0},
                        {0, 0, 9, 0, 0, 20},
                        {0, 0, 0, 7, 0, 4},
                        {0, 0, 0, 0, 0, 0}
    };
    cout << "The maximum possible flow is " << fordFulkerson(graph, 0, 5);
    return 0;
} 