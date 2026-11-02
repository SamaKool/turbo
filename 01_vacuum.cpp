#include <iostream>
#include <string>
using namespace std;

string action(const string& location, const string& status) {
    if (status == "Dirty") return "Suck";
    return location == "Left" ? "Right" : "Left";
}

void run(string left, string right, string location, int maxSteps = 10) {
    cout << "Initial: [" << left << "] [" << right << "] at " << location << '\n';
    for (int step = 1; step <= maxSteps; ++step) {
        string status = location == "Left" ? left : right;
        string move = action(location, status);
        cout << "Step " << step << ": percept=(" << location << "," << status
             << "), action=" << move << '\n';

        if (move == "Suck") (location == "Left" ? left : right) = "Clean";
        else location = move;

        cout << "  [" << left << "] [" << right << "] at " << location << '\n';

        if (left == "Clean" && right == "Clean") {
            cout << "Goal achieved\n";
            break;
        }
    }
}

int main() {
    cout << "Start on left:\n";
    run("Dirty", "Dirty", "Left");
    
    cout << "Start on right:\n";
    run("Dirty", "Dirty", "Right");
}
