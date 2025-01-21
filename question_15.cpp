#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

// Directions: East (0), West (1), South (2), North (3)
int dx[] = { 0, 0, 1, -1 };
int dy[] = { 1, -1, 0, 0 };

// Helper function to calculate the number of similar IDs for a target ID
int calculateSimilarCount(const vector<int>& freq, int targetID) {
    int count = 0;
    if (targetID > 0) count += freq[targetID - 1];
    count += freq[targetID];
    if (targetID < 15) count += freq[targetID + 1];
    return count;
}

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<int> compounds(N);
    for (int i = 0; i < N; ++i) {
        cin >> compounds[i];
    }
    
    // Sliding window frequency count
    vector<int> freq(16, 0); // Frequency of IDs 0 through 15 in the current window
    
    // Initialize the first window's frequency count
    for (int i = 0; i < M; ++i) {
        freq[compounds[i]]++;
    }

    int maxSimilar = 0;

    // For the first window, calculate the number of similar IDs for each target ID
    for (int targetID = 0; targetID < 16; ++targetID) {
        int currentMax = calculateSimilarCount(freq, targetID);
        if (currentMax > maxSimilar) {
            maxSimilar = currentMax;
				}
    }

    // Slide the window across the entire array
    for (int start = 1; start <= N - M; ++start) {
        // Remove the element that is sliding out of the window
        freq[compounds[start - 1]]--;
        // Add the new element that is sliding into the window
        freq[compounds[start + M - 1]]++;

        // For the new window, calculate the number of similar IDs for each target ID
        for (int targetID = 0; targetID < 16; ++targetID) {
            int currentMax = calculateSimilarCount(freq, targetID);
            if (currentMax > maxSimilar) {
                maxSimilar = currentMax;
            }
        }
    }

    // Output the result
    cout << maxSimilar << endl;
    
    return 0;
}
