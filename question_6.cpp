#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> masses(N);
    for (int i = 0; i < N; i++) {
        cin >> masses[i];
    }

    long long validPairs = 0;

    // Step 1: Count neighboring valid pairs
    validPairs += N - 1;  // Every consecutive pair (i, i+1) is valid

    // Step 2: Count non-adjacent valid pairs (i, j) where j > i + 1
    for (int i = 0; i < N; i++) {
        for (int j = i + 2; j < N; j++) {  // Start from j = i+2 to skip adjacent pairs
            bool valid = true;
            // Check if all intermediate planets' masses are smaller than both masses[i] and masses[j]
            for (int k = i + 1; k < j; k++) {
                if (masses[k] >= masses[i] || masses[k] >= masses[j]) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                validPairs++;
            }
        }
    }

    // Output the total valid pairs count
    cout << validPairs << endl;

    return 0;
}
