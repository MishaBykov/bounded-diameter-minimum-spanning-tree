//
// Created by misha on 25.11.18.
//
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

vector<int> p;

int dist(pair<int, int>& v1, pair<int, int>& v2)
{
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

void bfs(int ind_v, vector< vector<int>>& vvs)
{
    queue<pair<int, int>> queue;
    queue.push(pair<int, int>(ind_v, 0));
    vector<bool> vf;
    vf.resize(vvs.size(), true);
    pair<int, int> v_order;
    while(!queue.empty())
    {
        v_order = queue.front();
        vf[ind_v] = false;
        for(auto i : vvs[queue.front().first])
        {
            if(vf[queue.front().first])
                queue.push(pair<int, int>(i, queue.front().second + 1));
        }
        queue.pop();
    }
    vf.clear();
    vf.resize(vvs.size(), true);

}

int dsu_get (int v) {
    return (v == p[v]) ? v : (p[v] = dsu_get (p[v]));
}

void dsu_unite (int a, int b) {
    a = dsu_get (a);
    b = dsu_get (b);
    if (rand() & 1)
        swap (a, b);
    if (a != b)
        p[a] = b;
}

int main()
{
    unsigned long n;
    unsigned long m;
    vector< pair<int, pair<int, int>>> g; // вес - вершина 1 - вершина 2
    vector< vector<int>> vvs; // вершина - вершины
    vector< pair<int, int>> v;

    fstream file;
    file.open("../input/Taxicab_100.txt");

//    ... чтение графа ...
    file >> n;

    for(int i = 0; i < n; i++)
    {
        v.emplace_back();
        file >> v.back().first >> v.back().second;
        for(int j = i-1; j >= 0; j--)
        {
            g.emplace_back();
            g.back().first = dist(v[i], v[j]);
            g.back().second.first = i;
            g.back().second.second = j;
        }
    }

    m = g.size();
    int cost = 0;
    vector < pair<int, int> > res;

    sort(g.begin(), g.end());
    p.resize(n);
    for (int i=0; i<n; ++i)
        p[i] = i;
    for (int i=0; i<m; ++i) {
        int a = g[i].second.first,  b = g[i].second.second,  c = g[i].first;
        if (dsu_get(a) != dsu_get(b)) {
            cost += c;
            res.push_back (g[i].second);
            dsu_unite (a, b);
        }
    }

    vvs.resize(n);
    for(auto i : res)
    {
        vvs[i.first].push_back(i.second);
        vvs[i.second].push_back(i.first);
    }

    cout << res.size();

    return 0;
}


