#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <cmath>
#include <algorithm>

using namespace std;

int count_areas(int N, const string& path) {
    // Initialize coordinates
    int x = 0, y = 0;
    vector<pair<int, int>> vertices = {{x, y}};

    // Direction mappings for East(0), West(1), South(2), North(3)
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    // Track all visited coordinates and edges
    set<tuple<int, int, int, int>> lines;

    // Follow the path and collect vertices and edges
    for (char d : path) {
        int direction = d - '0';
        int next_x = x + dx[direction];
        int next_y = y + dy[direction];

        // Add edge (both directions to handle both ways)
        lines.insert(make_tuple(x, y, next_x, next_y));
        lines.insert(make_tuple(next_x, next_y, x, y));

        // Update current position and add to vertices
        x = next_x;
        y = next_y;
        vertices.push_back({x, y});
    }

    // Calculate area using shoelace formula
    int area = 0;
    int j = vertices.size() - 1;
    for (int i = 0; i < vertices.size(); ++i) {
        area += (vertices[j].first + vertices[i].first) * (vertices[j].second - vertices[i].second);
        j = i;
    }
    area = abs(area) / 2;

    // Count unique vertices and edges
    set<pair<int, int>> unique_vertices(vertices.begin(), vertices.end());
    int unique_edges = lines.size() / 2;  // Divide by 2 as we stored both directions

    // Using Euler's formula: V - E + F = 1 + L
    // where V is vertices, E is edges, F is faces, L is number of enclosed areas
    // Therefore: L = E - V + 1 + (F - 1) = E - V + F
    int regions = unique_edges - unique_vertices.size() + 2;

    // The number of enclosed regions is the total regions minus the outer region
    int enclosed_areas = max(0, regions - 1);
    return enclosed_areas;
}

int main() {
    int N;
    cin >> N;
    string path;
    cin >> path;

    int result = count_areas(N, path);
    cout << result << endl;

    return 0;
}
