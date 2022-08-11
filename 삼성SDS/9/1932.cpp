#include <iostream>
#include <vector>
using namespace std;
vector<vector<int>> dp;
vector<vector<int>> inputs;

int n;


int max(int a, int b){
	return a > b ? a : b;
}

int main(void){
	cin >> n;
	inputs.push_back(vector<int>());
	dp.push_back(vector<int>());
	
	for (int i = 1 ; i <= n ; i++){
		inputs.push_back(vector<int> ());
		inputs[i].push_back(0);
		dp.push_back(vector<int>());
		dp[i].push_back(0);
		
		for (int j = 1 ; j <= i ; j++){
			int input;
			cin >> input;
			inputs[i].push_back(input);
			dp[i].push_back(0);
		}
	}
	dp[1][1] = inputs[1][1];
	for (int i = 1; i < n ; i++){
		for (int j = 1; j <= i ; j++){
			
			dp[i+1][j] = max(dp[i][j] + inputs[i+1][j], dp[i+1][j]);
			dp[i+1][j+1] = max(dp[i][j] + inputs[i+1][j+1], dp[i+1][j+1]);		
		}
	}
	
	int result = 0;
	
	for (int i = 1; i <= n ; i++){
		result = max(result, dp[n][i]);
	}
	cout << result;
} 
