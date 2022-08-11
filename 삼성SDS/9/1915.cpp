#include <iostream>
#include <cmath>
using namespace std;

int matrix [1010][1010];
long dp[1010][1010];
int n, m;

int min(int a, int b){
	return a>b?b:a;
}

int max(int a, int b){
	return a > b ? a : b;
}

int main(void){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	
	cin >> n >> m;
	for (int i = 1; i <= n ; i++){
		for (int j = 1; j <= m ; j++){
			dp[i][j] = 0;
			char input;
			cin >> input;
			matrix[i][j] = input - '0';
		}
	}
	
	long result = 0;
	
	for (int i = 1; i <= n ; i++){
		for (int j = 1; j <=n; j++){
			if (matrix[i][j]==1) {
				dp[i][j] = min(dp[i-1][j-1], min(dp[i-1][j], dp[i][j-1])) + 1;
				result = max(result, dp[i][j]);
			}
		}
	}
	
	cout << (int)pow(result,2);
}
