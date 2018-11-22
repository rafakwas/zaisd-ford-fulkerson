using namespace std;

#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
#include<iostream>
#include<list>
#include <cmath>

#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>

int V;
int S;
int T;

class Edge {
public:
    int u, v;
    int length;

    Edge() {}

    Edge(int _u, int _v, int _length) {
        u = _u;
        v = _v;
        length = _length;
    };
};


int** alloc(int V) {
    auto ** array = new int*[V];
    for(int i = 0; i < V; ++i)
    {
        array[i] = new int[V];
    }
    return array;
}


bool dfs(int** rGraph, int x, int t, int * parent, bool * visited){
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

bool dfs(int** rGraph, int s, int t, int parent[])
{
    parent[s] = -1;
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    return dfs(rGraph, s, t, parent, visited);
}

int min_flow(int **rGraph, int s, int t, const int parent[]) {
    int path_flow = INT_MAX;
    for (int v=t; v!=s; v=parent[v])
    {
        int u = parent[v];
        path_flow = min(path_flow, rGraph[u][v]);
    }
    return path_flow;
}

int fordFulkerson(int **graph, int s, int t)
{
    int **rGraph = alloc(V);
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];
    int parent[V];
    int max_flow = 0;
    while (dfs(rGraph, s, t, parent))
    {
        int path_flow = min_flow(rGraph,s,t,parent);
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


int** read_matrix(char* file) {
    ifstream myfile ("1.txt");
    int **matrix = nullptr;
    int vertice = 0;
    string line;
    if (myfile.is_open()) {
        string item;
        vector <string> splittedStrings;
        int file_line = 0;
        while (getline(myfile, line)) {
            stringstream ss(line);
            if (file_line == 0) {
                V = atoi(line.c_str());
            } else if (file_line == 1) {
                S = atoi(line.c_str());
                matrix = alloc(V);
            }
            else if (file_line ==2) {
                T = atoi(line.c_str());
            } else {
                while (getline(ss, item, ' ')) {
                    splittedStrings.push_back(item);
                }
                for (int i = 0; i < splittedStrings.size(); ++i) {
                    matrix[vertice][i] = stoi(splittedStrings.at(i));
                }
                splittedStrings.clear();
                ++vertice;
            }
            ++file_line;
        }
        myfile.close();
    } else cout << "Unable to open file";
    return matrix;
}


int main(int argc, char* argv[])
{
    int **graph = read_matrix(argv[1]);
    cout << "Maximum flow " << fordFulkerson(graph, S, T);
    return 0;
}
