#include <array>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using State = array<int, 9>;

const State start = {2, 8, 3, 1, 6, 4, 7, 0, 5};
const State goal  = {1, 2, 3, 8, 0, 4, 7, 6, 5};

int heuristic(const State& s) {
    int h = 0;
    for (int i = 0; i < 9; ++i) if (s[i]) {
        int j = find(goal.begin(), goal.end(), s[i]) - goal.begin();
        h += abs(i / 3 - j / 3) + abs(i % 3 - j % 3);
    }
    return h;
}

void show(const State& s) {
    for (int i = 0; i < 9; ++i) {
        cout << (s[i] ? to_string(s[i]) : "_") << " ";
        if (i % 3 == 2) cout << "\n";
    }
}

int main() {
    using Node = tuple<int, int, State>; // f, g, state
    priority_queue<Node, vector<Node>, greater<Node>> open;
    map<State, int> bestG;
    bestG[start] = 0;
    
    map<State, pair<State, string>> parent;
    set<State> closed;
    
    open.push(Node(heuristic(start), 0, start));
    
    const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    const string names[] = {"Up", "Down", "Left", "Right"};
    
    while (!open.empty()) {
        int f, g;
        State s;
        tie(f, g, s) = open.top(); 
        open.pop();
        
        if (closed.count(s)) continue;
        closed.insert(s);
        
        if (s == goal) {
            vector<pair<State, string>> path;
            path.push_back(make_pair(s, string("Goal State")));
            
            while (path.back().first != start) {
                State previous;
                string move;
                tie(previous, move) = parent.at(path.back().first);
                path.back().second = move;
                path.push_back(make_pair(previous, string("")));
            }
            reverse(path.begin(), path.end());
            
            cout << "Moves: " << path.size() - 1 << ", explored: " << closed.size() << "\n\n";
            for (size_t i = 0; i < path.size(); ++i) { 
                if (i == 0) {
                    cout << "Start Configuration:\n";
                } else {
                    cout << "Move " << i << ": " << path[i].second << "\n";
                }
                show(path[i].first);
                cout << "\n";
            }
            return 0;
        }
        
        int zero = find(s.begin(), s.end(), 0) - s.begin();
        for (int i = 0; i < 4; ++i) {
            int r = zero / 3 + dr[i], c = zero % 3 + dc[i];
            if (r < 0 || r >= 3 || c < 0 || c >= 3) continue;
            State next = s;
            swap(next[zero], next[3 * r + c]);
            if (closed.count(next)) continue;
            if (!bestG.count(next) || g + 1 < bestG[next]) {
                bestG[next] = g + 1;
                parent[next] = make_pair(s, names[i]);
                open.push(Node(g + 1 + heuristic(next), g + 1, next));
            }
        }
    }
    cout << "No solution\n";
}