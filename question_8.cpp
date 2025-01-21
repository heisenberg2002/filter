#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct Player {
    long long position;  // Initial position
    long long speed;     // Speed
    int index;           // Player index (1-based)
};

int main() {
    int N;  // Number of players
    long long T;  // Time in minutes
    cin >> N >> T;

    vector<Player> players(N);
    for (int i = 0; i < N; i++) {
        cin >> players[i].position >> players[i].speed;
        players[i].index = i + 1;  // Player index is 1-based
    }

    // Stack to determine groups (stores front-runners)
    stack<Player> groups;

    // Traverse players from back to front
    for (int i = N - 1; i >= 0; i--) {
        // Calculate final position after T minutes
        long long final_position = players[i].position + players[i].speed * T;

        // If the stack is empty or this player cannot catch up to the current group, form a new group
        if (groups.empty() || final_position < (groups.top().position + groups.top().speed * T)) {
            groups.push(players[i]);
        }
    }

    // Output the results
    cout << groups.size() << endl;  // Number of groups
    vector<int> front_runners;
    while (!groups.empty()) {
        front_runners.push_back(groups.top().index);  // Collect front-runner indices
        groups.pop();
    }

    // Output front-runner indices in reverse order (from front to back)
    for (int i = front_runners.size() - 1; i >= 0; i--) {
        cout << front_runners[i] << " ";
    }
    cout << endl;

    return 0;
}
