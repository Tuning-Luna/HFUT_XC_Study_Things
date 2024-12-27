#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 1e9;
const int MAXN = 10005;

struct Edge {
    int to, cap, flow, reverse_index;
};

vector<Edge> G[MAXN];
void addEdge(int u, int v, int cap) {
    Edge a = { v, cap, 0, (int)G[v].size() };
    Edge b = { u, 0, 0, (int)G[u].size() };
    G[u].push_back(a);
    G[v].push_back(b);
}

bool bfs(int s, int sink, vector<int>& par, vector<int>& parEdg) {
    fill(par.begin(), par.end(), -1);
    queue<int> q;
    q.push(s);
    par[s] = s;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < G[u].size(); i++) {
            Edge& e = G[u][i];
            int v = e.to;
            if (par[v] == -1 && e.cap > e.flow) {
                par[v] = u;
                parEdg[v] = i;
                if (v == sink) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int helper(int s, int sink) {
    int max_flow = 0;
    vector<int> par(MAXN), parEdg(MAXN);

    while (bfs(s, sink, par, parEdg)) {
        int path_flow = INF;
        for (int v = sink; v != s; v = par[v]) {
            int u = par[v];
            Edge& e = G[u][parEdg[v]];
            path_flow = min(path_flow, e.cap - e.flow);
        }
        for (int v = sink; v != s; v = par[v]) {
            int u = par[v];
            Edge& e = G[u][parEdg[v]];
            e.flow += path_flow;
            G[v][e.reverse_index].flow -= path_flow;
        }
        max_flow += path_flow;
    }

    return max_flow;
}

int main() {
    int N, M;
    cin >> N >> M;

    for (int i = 0; i < M; ++i) {
        int u, v, cap;
        cin >> u >> v >> cap;
        addEdge(u, v, cap);
    }

    int ans = helper(1, N);
    cout << ans << endl;

    return 0;
}
