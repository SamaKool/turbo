#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using State = pair<int,int>;
int main() {
    const int cap1 = 3, cap2 = 5, target = 4; // Same values as WATER.py.
    set<State> seen;
    vector<State> path;
    vector<string> actions;
    function<bool(State)> dfs = [&](State s) {
        if (seen.count(s)) return false;
        seen.insert(s); path.push_back(s);
        auto [a,b] = s;
        if (a == target || b == target) return true;
        int pour12 = min(a, cap2-b), pour21 = min(b, cap1-a);
        vector<pair<State,string>> next = {
            {{cap1,b},"Fill Jug 1"}, {{a,cap2},"Fill Jug 2"},
            {{0,b},"Empty Jug 1"}, {{a,0},"Empty Jug 2"},
            {{a-pour12,b+pour12},"Pour Jug 1 -> Jug 2"},
            {{a+pour21,b-pour21},"Pour Jug 2 -> Jug 1"}
        };
        for (auto& [state, move] : next) {
            if (state == s) continue;
            actions.push_back(move);
            if (dfs(state)) return true;
            actions.pop_back();
        }
        path.pop_back();
        return false;
    };
    if (!dfs({0,0})) {cout << "No solution\n"; return 0;}
    for (size_t i=0; i<path.size(); ++i)
        cout << i << ": " << (i ? actions[i-1] : "Start") << " -> ("
             << path[i].first << "," << path[i].second << ")\n";
    cout << "Steps: " << path.size()-1 << '\n';
}
