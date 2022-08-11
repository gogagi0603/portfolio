#include <iostream>

using namespace std;

int stair[301];
int dp[301];
int n;
int max(int a, int b){
	return a > b ? a : b;
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n;
	for (int i = 0 ; i < n ; i++){
		int input;
		cin >> input;
		stair[i] = input;
	}
	
	dp[0] = stair[0];
	dp[1] = max(dp[0] + stair[1], stair[1]);
	dp[2] = max(dp[0] + stair[2] , stair[1] + stair[2]);
	
	for (int i = 3; i < n ; i ++){
		dp[i] = max(dp[i-2] + stair[i], stair[i] + stair[i-1] + dp[i-3]);
	}
	
	cout << dp[n-1];
	return 0;
}
