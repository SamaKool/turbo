#include <array>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;
int main() {
    array<int,8> column;
    column.fill(-1);
    function<bool(int)> solve = [&](int row) {
        if (row == 8) return true;
        for (int col = 0; col < 8; ++col) {
            bool safe = true;
            for (int r = 0; r < row; ++r)
                if (column[r] == col || abs(column[r]-col) == row-r) safe = false;
            if (!safe) continue;
            column[row] = col;
            cout << "Place (" << row+1 << "," << col+1 << ")\n";
            if (solve(row+1)) return true;
            cout << "Backtrack (" << row+1 << "," << col+1 << ")\n";
            column[row] = -1;
        }
        return false;
    };
    if (solve(0)) {
        cout << "Solution:\n";
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) cout << (column[r]==c ? "Q " : ". ");
            cout << '\n';
        }
    }
}
