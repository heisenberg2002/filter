#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

// Function to perform Depth-First Search (DFS) to mark zones
void dfs(vector<vector<char>>& grid, vector<vector<bool>>& visited, int i, int j, int N, char owner) {
    stack<pair<int, int>> s;
    s.push({i, j});
    visited[i][j] = true;
    
    // Directions for adjacent cells: up, down, left, right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    while (!s.empty()) {
        int x = s.top().first;
        int y = s.top().second;
        s.pop();
        
        for (auto& dir : directions) {
            int nx = x + dir[0], ny = y + dir[1];
            if (nx >= 0 && nx < N && ny >= 0 && ny < N && !visited[nx][ny] && grid[nx][ny] == owner) {
                visited[nx][ny] = true;
                s.push({nx, ny});
            }
        }
    }
}

void solve() {
    // Read input
    int N;
    cin >> N;
    vector<vector<char>> grid(N, vector<char>(N));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    // Initialize visited matrix to track which cells are already part of a zone
    vector<vector<bool>> visited(N, vector<bool>(N, false));

    // Dictionary to store the zone and land cell counts for each owner
    int zone_count[3] = {0};  // R, G, B zones count
    int land_count[3] = {0};  // R, G, B total land count

    // Map owners to indices
    char owners[3] = {'R', 'G', 'B'};
    int owner_index[256] = {0}; // map 'R'->0, 'G'->1, 'B'->2
    owner_index['R'] = 0;
    owner_index['G'] = 1;
    owner_index['B'] = 2;

    // Count zones and land cells for each owner
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char owner = grid[i][j];
            int idx = owner_index[owner];
            
            if (!visited[i][j]) {
                dfs(grid, visited, i, j, N, owner);
                zone_count[idx]++;  // Increment zone count for this owner
            }
            land_count[idx]++;  // Increment land count for this owner
        }
    }

    // Find the owner with the maximum number of zones
    // If there's a tie, use land cell count as the tiebreaker
    int max_zones = -1;
    int max_owner = -1;
    
    for (int i = 0; i < 3; i++) {
        if (zone_count[i] > max_zones || (zone_count[i] == max_zones && land_count[i] > land_count[max_owner])) {
            max_zones = zone_count[i];
            max_owner = i;
        }
    }

    // Output the result
    cout << owners[max_owner] << " " << max_zones << endl;
}

int main() {
    solve();
    return 0;
}
