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

int main()
{
    unsigned long n, d;
    vector< pair<int, int>> v;

    fstream in;
    in.open("../input/Taxicab_100.txt");

    pair<int, int> p_min;
    pair<int, int> p_max;

//    ... чтение графа ...
    in >> n >> d;
    v.emplace_back();
    in >> v.back().first >> v.back().second;
    p_min.first = v.back().first;
    p_min.second = v.back().second;
    p_max.first = v.back().first;
    p_max.second = v.back().second;

    for(int i = 1; i < n; i++) {
        v.emplace_back();
        in >> v.back().first >> v.back().second;
        if(v.back().first > p_max.first)
            p_max.first = v.back().first;
        if(v.back().second > p_max.second)
            p_max.second = v.back().second;
        if(p_min.first > v.back().first)
            p_min.first = v.back().first;
        if(p_min.second > v.back().second)
            p_min.second = v.back().second;
    }

    pair<int, int> p_center((p_max.first+p_min.first)/2, (p_max.second+p_min.second)/2);
    int r = dist(p_center, p_max)/2;
    int v_center = 0;
    for(int i = 1; i < v.size(); i++){
       if(dist(v[i], p_center) < dist(v[v_center], p_center))
           v_center = i;
    }

    vector<bool> v_in_r(v.size(), false);
    v_in_r[v_center] = true;
    vector<pair<int, pair<int, int>>> result_cost_v_v;

    for(int i = 0; i < v.size(); i++){
        if(dist(v[v_center], v[i]) <= r && i != v_center){
            v_in_r[i] = true;
            result_cost_v_v.emplace_back();
            result_cost_v_v.back().first = dist(v[v_center], v[i]);
            result_cost_v_v.back().second.first = v_center;
            result_cost_v_v.back().second.second = i;
        }
    }
    for(int i = 0; i < v.size(); i++ ){
        if(v_in_r[i])
            continue;
        int cost_min = dist(v[i], v[v_center]);
        int v_min = v_center;
        for(int j = 0; j < v.size(); j++){
            if(!v_in_r[j] || i == j)
                continue;
            if(cost_min > dist(v[i], v[j])){
                cost_min = dist(v[i], v[j]);
                v_min = j;
            }
        }
        result_cost_v_v.emplace_back();
        result_cost_v_v.back().first = cost_min;
        result_cost_v_v.back().second.first = i;
        result_cost_v_v.back().second.second = v_min;
    }

    int all_cost = 0;
    for(auto i : result_cost_v_v){
        all_cost += i.first;
    }

    fstream result;
    result.open("../result_100.txt");
    result << "c Вес дерева = " << all_cost << ", диаметр = " << 4 << ',' << endl;
    result << "c число вершин и ребер" << endl;
    result << "p edge " << n << ' ' << result_cost_v_v.size() << endl;
    result << "c ребра" << endl;
    for (auto &i : result_cost_v_v) {
        result << "e " << i.second.first+1 << ' ' << i.second.second+1 << endl;
    }

    return 0;
}


