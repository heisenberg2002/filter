#include <iostream>
using namespace std;

int N; // Size of the landscape grid
int a[10 + 10][10 + 10]; // Landscape grid
int X1, X2, X3; // Hidden picture numbers (three numbers)

// Directions for checking in 8 directions: (up, up-right, right, down-right, down, down-left, left, up-left)
int di[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dj[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int Solve(void) {
    int count = 0;
    for (int i = 0; i < N; i++) { // Iterate through each cell in the grid
        for (int j = 0; j < N; j++) {
            if (a[i][j] != X1) continue; // The first number must match X1

            // Check all 8 directions for X2 and X3
            for (int k = 0; k < 8; k++) {
                int ni = i + di[k]; // Next cell in the current direction
                int nj = j + dj[k];

                if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue; // Boundary check
                if (a[ni][nj] != X2) continue; // Check for X2

                // Move further in the same direction for X3
                int nni = ni + di[k];
                int nnj = nj + dj[k];

                if (nni < 0 || nni >= N || nnj < 0 || nnj >= N) continue; // Boundary check
                if (a[nni][nnj] != X3) continue; // Check for X3

                // If all three numbers are found in the correct order, increment count
                count++;
            }
        }
    }
    return count;
}

void InputData(void) {
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> a[i][j];
        }
    }
    cin >> X1 >> X2 >> X3;
}

int main(void) {
    int ans;
    InputData();
    ans = Solve();
    cout << ans;
    return 0;
}
