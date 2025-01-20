#include <iostream>
using namespace std;

int R, C; // 격자의 행의 수, 열의 수
int Mat[50][50]; // 격자 데이터


void InputData(void) {
	cin >> R >> C;
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			cin >> Mat[r][c];
		}
	}
}

void OutputData(void) {

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cout << Mat[i][j] << " ";
		}
		cout << endl;
	}
}

#define SWAP(a, b) {int temp = a; a = b; b = temp;}

void Rotate(int sr, int sc, int er, int ec) {
    // Check if the bounds are valid for rotation
    if (sr >= er || sc >= ec) return;

    int a = Mat[sr][sc]; // Temporarily store the first element

    // Move elements from the left column up
    for (int r = sr + 1; r <= er; r++) {
        SWAP(Mat[r][sc], a);
    }

    // Move elements from the bottom row left
    for (int c = sc + 1; c <= ec; c++) {
        SWAP(Mat[er][c], a);
    }

    // Move elements from the right column down
    for (int r = er - 1; r >= sr; r--) {
        SWAP(Mat[r][ec], a);
    }

    // Move elements from the top row right
    for (int c = ec - 1; c >= sc; c--) {
        SWAP(Mat[sr][c], a);
    }
}


void Solve(void) {
	int n = R > C ? R : C;
	for (int i = 0; i < n; i++) {
		Rotate(0 + i, 0 + i, R - 1 - i, C - 1 - i);
	}
}

int main(void) {
	InputData();
	Solve();
	OutputData();
	return 0;
}
