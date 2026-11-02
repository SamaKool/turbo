#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;
using State = vector<vector<int>>;

int main() {
    int n;
    cout << "Number of disks (1-8): ";
    if (!(cin >> n) || n < 1 || n > 8) { cerr << "Use 1-8 disks\n"; return 1; }
    State start(3), goal(3);
    for (int d = n; d >= 1; --d) { start[0].push_back(d); goal[2].push_back(d); }
    queue<State> q; q.push(start);
    map<State, pair<State, pair<int,int>>> parent;
    parent[start] = {start, {-1,-1}};
    while (!q.empty()) {
        State s = q.front(); q.pop();
        if (s == goal) break;
        for (int from = 0; from < 3; ++from) if (!s[from].empty())
            for (int to = 0; to < 3; ++to) {
                if (from == to || (!s[to].empty() && s[to].back() < s[from].back())) continue;
                State next = s;
                next[to].push_back(next[from].back()); next[from].pop_back();
                if (!parent.count(next)) { parent[next] = {s, {from,to}}; q.push(next); }
            }
    }
    vector<pair<int,int>> moves;
    for (State s = goal; s != start; s = parent.at(s).first)
        moves.push_back(parent.at(s).second);
    cout << "Shortest solution: " << moves.size() << " moves\n";
    for (auto it = moves.rbegin(); it != moves.rend(); ++it)
        cout << "Peg " << char('A'+it->first) << " -> Peg " << char('A'+it->second) << '\n';
}
