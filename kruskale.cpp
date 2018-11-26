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

vector<vector<int>> p;

int dist(pair<int, int>& v1, pair<int, int>& v2)
{
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

int double_bfs(int ind_v, vector< vector<int>>& vvs)
{
    static bool run = false;

    queue<pair<int, int>> queue;
    queue.push(pair<int, int>(ind_v, 0));
    vector<bool> vf;
    vf.resize(vvs.size(), true);
    vf[0] = false;
    pair<int, int> v_order_max(0, 0);
    while(!queue.empty())
    {
        if(queue.front().second > v_order_max.second)
        {
            v_order_max = queue.front();
        }
        for(auto i : vvs[queue.front().first])
        {
            if(vf[i])
            {
                queue.push(pair<int, int>(i, queue.front().second + 1));
                vf[i] = false;
            }
        }
        queue.pop();
    }

    if(run)
    {
        run = false;
        return v_order_max.second;
    }
    run = true;
    return double_bfs(v_order_max.first, vvs);
}

int diam(vector< pair<int, pair<int, int>>> edges, vector<int>& ost, unsigned long n)
{
    vector< vector<int>> vvs; // вершина - вершины
    vvs.resize(n);
    for(auto i : ost)
    {
        vvs[edges[i].second.first].push_back(edges[i].second.second);
        vvs[edges[i].second.second].push_back(edges[i].second.first);
    }
    return double_bfs(0, vvs);
}

//  обращаться к текущему
int dsu_get (int v) {
    return (v == p.back()[v]) ? v : (p.back()[v] = dsu_get(p.back()[v]));
}

void dsu_unite (int a, int b) {
    a = dsu_get (a);
    b = dsu_get (b);
    if (rand() & 1)
        swap (a, b);
//    перед p[a] добавить новую строку vector
//    указатель на текущий
    if (a != b)
    {
        p.emplace_back();
        p.back() = p[p.size() - 2];
        p.back()[a] = b;
    }
}

void dsu_pop_back()
{
    p.pop_back();
}

int main()
{
    unsigned long n, m, d;
    vector< pair<int, pair<int, int>>> g; // вес - вершина 1 - вершина 2
    vector< vector<int>> vvs; // вершина - вершины
    vector< pair<int, int>> v;

    fstream in, out;
    out.open("../out.txt");
    in.open("../input/input.txt");

//    ... чтение графа ...
    in >> n >> d;

    for(int i = 0; i < n; i++)
    {
        v.emplace_back();
        in >> v.back().first >> v.back().second;
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
    vector<int> res;

    sort(g.begin(), g.end());
    p.emplace_back();
    p.back().resize(n);
    for (int i=0; i<n; ++i)
        p.back()[i] = i;

    int i = 0;
    while(true)
    {
        if(i == g.size())
        {
            if(res.empty())
                break;
            cost -= g[res.back()].first;
            i = res.back() + 1;
            res.pop_back();
            dsu_pop_back();
            continue;
        }
        int a = g[i].second.first,  b = g[i].second.second,  c = g[i].first;
        if (dsu_get(a) != dsu_get(b))
        {
            cost += c;
            res.push_back(i);
            dsu_unite (a, b);
            if(res.size() == n-1)
            {
                int dim = diam(g, res, n);
                out << dim << endl;
                for(int j = 0; j < res.size(); j++){
                    out << g[j].second.first << ' ' << g[j].second.second << endl;
                }
                out << "-----------------------" << endl;
                if(dim <= d )
                    break;
                else
                {
                    cost -= c;
                    i = res.back();
                    res.pop_back();
                    dsu_pop_back();
                }
            }
        }
        i++;
    }

    for(int i = 0; i < res.size(); i++){
        cout << g[i].second.first << ' ' << g[i].second.second << endl;
    }

    return 0;
}


