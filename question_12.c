#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Check if all foreign matters can be absorbed using at most K equipment with force V
bool isValid(int V, const vector<int>& positions, int N, int K) {
    int used = 0;  // Number of times the equipment is used
    int i = 0;     // Index of the first uncovered foreign matter

    while (i < N) {
        used++;
        int range = positions[i] + 2 * V;  // Range covered by placing equipment
        while (i < N && positions[i] <= range) {
            i++;  // Move to the next uncovered foreign matter
        }
        if (used > K) return false;  // Exceeded maximum uses
    }

    return true;
}

int main() {
    // Input
    int N, K;
    cin >> N >> K;
    vector<int> positions(N);
    for (int i = 0; i < N; i++) {
        cin >> positions[i];
    }

    // Sort the positions
    sort(positions.begin(), positions.end());

    // Binary search for the minimum V
    int left = 0, right = positions[N - 1] - positions[0];
    int result = right;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (isValid(mid, positions, N, K)) {
            result = mid;  // Try for smaller V
            right = mid - 1;
        } else {
            left = mid + 1;  // Increase V
        }
    }

    // Output the result
    cout << result << endl;

    return 0;
}
