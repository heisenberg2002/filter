#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <climits>
using namespace std;

const int INF = 1e9;
const int MAX_POINTS = 15;

int R, C;
vector<string> grid;
vector<pair<int, int>> points;
int dist[MAX_POINTS][MAX_POINTS];
int dp[1 << MAX_POINTS][MAX_POINTS];

// Directions for BFS
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// Perform BFS to calculate shortest paths between points
void bfs(int idx, int start_x, int start_y) {
    vector<vector<int>> visited(R, vector<int>(C, -1));
    queue<pair<int, int>> q;
    q.push({start_x, start_y});
    visited[start_x][start_y] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (nx >= 0 && nx < R && ny >= 0 && ny < C && visited[nx][ny] == -1 && grid[nx][ny] != '*') {
                visited[nx][ny] = visited[x][y] + 1;
                q.push({nx, ny});

                // Check if this position is a point of interest
                for (int i = 0; i < points.size(); i++) {
                    if (points[i].first == nx && points[i].second == ny) {
                        dist[idx][i] = visited[nx][ny];
                    }
                }
            }
        }
    }
}

// Solve TSP using bitmask DP
int solveTSP(int n) {
    int full_mask = (1 << n) - 1;

    // Initialize DP table
    for (int mask = 0; mask <= full_mask; mask++) {
        for (int i = 0; i < n; i++) {
            dp[mask][i] = INF;
        }
    }

    dp[1][0] = 0; // Start at 'S'

    for (int mask = 1; mask <= full_mask; mask++) {
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue; // Skip if point `i` is not visited

            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue; // Skip if point `j` is already visited

                int next_mask = mask | (1 << j);
                dp[next_mask][j] = min(dp[next_mask][j], dp[mask][i] + dist[i][j]);
            }
        }
    }

    // Calculate the minimum cost to return to 'S'
    int result = INF;
    for (int i = 0; i < n; i++) {
        result = min(result, dp[full_mask][i] + dist[i][0]);
    }
    return result;
}

int main() {
    cin >> R >> C;
    grid.resize(R);
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
    }

    // Find points of interest
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'S') {
                points.push_back({i, j}); // Add the starting point
            } else if (isdigit(grid[i][j])) {
                points.push_back({i, j}); // Add exploration points
            }
        }
    }

    int n = points.size();

    // Initialize distances
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i][j] = INF;
        }
    }

    // Perform BFS for each point
    for (int i = 0; i < n; i++) {
        bfs(i, points[i].first, points[i].second);
    }

    // Solve the TSP problem
    int result = solveTSP(n);

    cout << result << endl;

    return 0;
}
