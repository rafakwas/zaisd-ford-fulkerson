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
int T;

class FlowEdge
{
private:
    int v, w;
    int c;
    int f;
public:

    FlowEdge() {

    }

    FlowEdge(int _v, int _w, int _capacity)
    {
        v = _v;
        w = _w;
        c = _capacity;
    };

    int from() {return v;}
    int to() { return w; }
    int other(int vertex) {
        if (vertex == v) return w;
        else if (vertex == w) return v;
    }
    int residualCapacityTo(int vertex) {
        if (vertex == v) return f;
        else if (vertex == w) return c - f;
    }
    void addResidualFlowTo(int vertex, int delta) {
        if (vertex == v) f -= delta;
        else if (vertex == w) f += delta;
    }
};

class FlowNetwork {
public:
    FlowNetwork() {

    }

    FlowNetwork(int _V)
    {
        V = _V;
        a = new vector<FlowEdge*>[V];
    }

    void addEdge(FlowEdge e)
    {
        int v = e.from();
        int w = e.to();
        a[v].push_back(new FlowEdge(e));
        a[w].push_back(new FlowEdge(e));
    }
    vector<FlowEdge* > adj(int v) { return a[v]; }
public:
    int V;
    vector< FlowEdge* > * a;
};

class FordFulkerson {

public:
    FordFulkerson(FlowNetwork G_, int s_, int t_)
    {
            G = G_;
            s = s_;
            t = t_;
    }
    void solve() {
        max_flow = 0;
        while (hasAugmentingPath(G, s, t))
        {
            int bottle = INT_MAX;
            for (int v = t; v != s; v = edgeTo[v]->other(v))
                bottle = min(bottle, edgeTo[v]->residualCapacityTo(v));
            for (int v = t; v != s; v = edgeTo[v]->other(v))
                edgeTo[v]->addResidualFlowTo(v, bottle);
            max_flow += bottle;
        }
    }
    int value() { return max_flow; }
private:
    FlowNetwork G;
    int s;
    int t;
    bool* marked; // true if s->v path in residual network
    FlowEdge** edgeTo; // last edge on s->v path
    int max_flow; // value of flow
    bool hasAugmentingPath(FlowNetwork G, int s, int t) {
        edgeTo = new FlowEdge*[G.V];
        marked = new bool[G.V];
        for (int i = 0; i < G.V; i++) {
            marked[i] = false;
        }
        queue<int> queue;
        queue.push(s);
        marked[s] = true;
        while (!queue.empty())
        {
            int v = queue.front();
            queue.pop();
            for (FlowEdge* e : G.adj(v))
            {
                int w = e->other(v);
                if (!marked[w] && (e->residualCapacityTo(w) > 0) )
                {
                    edgeTo[w] = e;
                    marked[w] = true;
                    queue.push(w);
                }
            }
        }
        return marked[t];
    }
};

FlowNetwork read_edges(char* file) {
    string line;
    ifstream infile("3.txt");
    int u, v, length;
    int counter = 0;
    FlowNetwork flowNetwork;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (counter == 0) {
            if (!(iss >> V)) { break; }
            FlowNetwork tmp = FlowNetwork(V);
            flowNetwork = tmp;
            counter++;
        } else if (counter == 1) {
            if (!(iss >> T)) { break; }
            counter++;
        } else {
            if (!(iss >> u >> v >> length)) { break; }
            flowNetwork.addEdge(FlowEdge(u,v,length));
        }
    }
    return flowNetwork;
}

int main(int argc, char* argv[])
{
    FlowNetwork flowNetwork = read_edges(argv[1]);
    FordFulkerson fordFulkerson = FordFulkerson(flowNetwork,0,T);
    fordFulkerson.solve();
    cout << "Maximum flow " << fordFulkerson.value();
    return 0;
}
