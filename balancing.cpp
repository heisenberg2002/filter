#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Function to calculate the minimum total quantity of products to be added
int balanceBox(int N, vector<vector<int>>& grid) {
    // Step 1: Calculate row sums
    vector<int> row_sums(N, 0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            row_sums[i] += grid[i][j];
        }
    }

    // Step 2: Calculate column sums
    vector<int> col_sums(N, 0);
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            col_sums[j] += grid[i][j];
        }
    }

    // Step 3: Find the maximum row sum and column sum to determine the target sum
    int target_sum = max(*max_element(row_sums.begin(), row_sums.end()), *max_element(col_sums.begin(), col_sums.end()));

    // Step 4: Calculate the total number of products to add
    int total_addition = 0;

    // Add the difference to the row sums
    for (int i = 0; i < N; i++) {
        total_addition += target_sum - row_sums[i];
    }

    // Add the difference to the column sums
    for (int j = 0; j < N; j++) {
        total_addition += target_sum - col_sums[j];
    }

    return total_addition/2;
}

int main() {
    int N;
    cin >> N; // Read the size of the box

    // Read the grid (N x N lattice)
    vector<vector<int>> grid(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    // Calculate the minimum number of products to be added
    int result = balanceBox(N, grid);

    // Output the result
    cout << result << endl;

    return 0;
}
