#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <vector>
using namespace std;
using Tour = vector<int>;
const vector<vector<int>> dist = {
    {0,10,15,20,25,30}, {10,0,35,25,20,15}, {15,35,0,30,10,20},
    {20,25,30,0,15,25}, {25,20,10,15,0,35}, {30,15,20,25,35,0}
};
int cost(const Tour& t) {
    int total = 0;
    for (int i = 0; i < 6; ++i) total += dist[t[i]][t[(i+1)%6]];
    return total;
}
void show(const Tour& t) {
    for (int city : t) cout << city << " -> ";
    cout << t[0] << " (distance " << cost(t) << ")\n";
}
int main() {
    mt19937 rng(42); // Fixed seed makes revision runs repeatable.
    Tour base = {0,1,2,3,4,5};
    set<Tour> beam;
    while (beam.size() < 2) {
        shuffle(base.begin()+1, base.end(), rng);
        beam.insert(base);
    }
    set<Tour> visited = beam;
    Tour best = *min_element(beam.begin(), beam.end(), [](auto& a, auto& b){return cost(a)<cost(b);});
    for (int step = 1; step <= 10; ++step) {
        set<Tour> candidates;
        for (const Tour& state : beam)
            for (int i = 1; i < 6; ++i) for (int j = i+1; j < 6; ++j) {
                Tour next = state; swap(next[i], next[j]);
                if (!visited.count(next)) candidates.insert(next);
            }
        if (candidates.empty()) break;
        visited.insert(candidates.begin(), candidates.end());
        vector<Tour> ranked(candidates.begin(), candidates.end());
        sort(ranked.begin(), ranked.end(), [](auto& a, auto& b){
            return cost(a) != cost(b) ? cost(a)<cost(b) : a<b;
        });
        beam.clear();
        for (int i = 0; i < min(2, (int)ranked.size()); ++i) beam.insert(ranked[i]);
        if (cost(ranked[0]) < cost(best)) best = ranked[0];
        cout << "Step " << step << ": "; show(ranked[0]);
    }
    cout << "Best: "; show(best);
}
