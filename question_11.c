#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>
#include <unordered_map>
using namespace std;

// Function to calculate the number of factors of a number
int count_factors(int num) {
    int count = 0;
    for (int i = 1; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            count++;
            if (i != num / i) { // Count the complement factor
                count++;
            }
        }
    }
    return count;
}

// Function to generate valid neighbors of a planet
vector<int> generate_neighbors(int planet, const unordered_map<int, int>& factor_count) {
    vector<int> neighbors;
    string planet_str = to_string(planet);

    for (int i = 0; i < 4; ++i) { // Modify each digit
        char original_digit = planet_str[i];
        for (char new_digit = '0'; new_digit <= '9'; ++new_digit) {
            if (new_digit == original_digit) continue;

            planet_str[i] = new_digit; // Change the digit
            int new_planet = stoi(planet_str);

            // Ensure the planet is valid and satisfies factor count condition
            if (new_planet >= 1000 && new_planet <= 9999 &&
                abs(factor_count.at(planet) - factor_count.at(new_planet)) <= 1) {
                neighbors.push_back(new_planet);
            }
        }
        planet_str[i] = original_digit; // Revert the digit change
    }
    return neighbors;
}

// BFS to find the shortest path from start to end
int bfs(int start, int end, const unordered_map<int, int>& factor_count) {
    queue<pair<int, int>> q; // {current planet, moves}
    unordered_set<int> visited;

    q.push({start, 0});
    visited.insert(start);

    while (!q.empty()) {
        auto [current, moves] = q.front();
        q.pop();

        if (current == end) {
            return moves;
        }

        for (int neighbor : generate_neighbors(current, factor_count)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push({neighbor, moves + 1});
            }
        }
    }
    return -1; // Should never reach here based on problem guarantees
}

int main() {
    // Input
    int S, E1, E2;
    cin >> S >> E1 >> E2;

    // Precompute factor counts for all 4-digit numbers
    unordered_map<int, int> factor_count;
    for (int i = 1000; i <= 9999; ++i) {
        factor_count[i] = count_factors(i);
    }

    // Find shortest paths
    int result1 = bfs(S, E1, factor_count);
    int result2 = bfs(S, E2, factor_count);

    // Output results
    cout << result1 << endl;
    cout << result2 << endl;

    return 0;
}
