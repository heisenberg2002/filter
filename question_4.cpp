// 수정전 로봇청소기 1회 사용 시작시간과 종료시간을 이용하여 요금을 계산한 코드

#include <iostream>
using namespace std;

char start_time[10];
char end_time[10];

void InputData(void)
{
	cin >> start_time;
	cin >> end_time;
}

int ConvertInt(char * t)
{
	return (t[0] - '0') * 10 + (t[1] - '0');
}

int ComputeTime(void)
{
	int s = ConvertInt(start_time) * 60 + ConvertInt(start_time + 3);
	int e = ConvertInt(end_time) * 60 + ConvertInt(end_time + 3);
	if (e < s) {
    e += 24 * 60; // 24 hours in minutes
  }

  return (e - s);
}

int Solve(void)
{
    int t = ComputeTime(); // Calculate the total duration in minutes

    // Case 1: If duration is less than 30 minutes, return the minimum fee of 500
    if (t < 30) return 500;

    // Case 2: Calculate the additional fee for durations greater than 30 minutes
    int additional_fee = ((t - 30) / 10) * 300;

    // Determine the total fee based on whether there is a remainder
    int total_fee;
    if ((t - 30) % 10 == 0)
        total_fee = 500 + additional_fee;
    else
        total_fee = 800 + additional_fee;

    // Case 3: Apply the maximum fee cap of 30,000
    if (total_fee > 30000)
        total_fee = 30000;

    return total_fee;
}


void OutputData(int sol)
{
	cout << sol;
}

int main(void)
{
	int sol = -1;
	InputData();// 입력

	sol = Solve();

	OutputData(sol);// 출력

	return 0;
}
