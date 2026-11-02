#include <array>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;
using State = array<int,9>;
const State start = {4,1,3,0,2,5,6,7,8};
const State goal  = {1,2,3,4,0,5,6,7,8};
void show(const State& s) {
    for (int i = 0; i < 9; ++i) cout << (s[i] ? to_string(s[i]) : "_")
                                      << (i%3 == 2 ? "\n" : " ");
}
int main() {
    queue<State> q; q.push(start);
    map<State, State> parent; parent[start] = start;
    const int dr[] = {-1,1,0,0}, dc[] = {0,0,-1,1};
    while (!q.empty()) {
        State s = q.front(); q.pop();
        if (s == goal) break;
        int zero = find(s.begin(),s.end(),0)-s.begin();
        for (int i = 0; i < 4; ++i) {
            int r = zero/3+dr[i], c = zero%3+dc[i];
            if (r < 0 || r >= 3 || c < 0 || c >= 3) continue;
            State next = s; swap(next[zero],next[3*r+c]);
            if (!parent.count(next)) {parent[next]=s; q.push(next);}
        }
    }
    if (!parent.count(goal)) {cout << "No solution\n"; return 0;}
    vector<State> path;
    for (State s = goal; s != start; s = parent.at(s)) path.push_back(s);
    path.push_back(start); reverse(path.begin(),path.end());
    cout << "Moves: " << path.size()-1 << '\n';
    for (const State& s : path) {show(s); cout << '\n';}
}
